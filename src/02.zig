const std = @import("std");
const expect = std.testing.expect;

const Tolerance = enum {
    no,
    yes,
};

fn loadReport(allocator: std.mem.Allocator, report: [][]u32, path: []const u8) !void {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var buffer: [1024]u8 = undefined;
    const reader = file.reader();
    var i: usize = 0;
    while (try reader.readUntilDelimiterOrEof(&buffer, '\n')) |line| {
        var levels_buffer: [100]u32 = undefined;
        const len = try splitLine(line, &levels_buffer);
        report[i] = try allocator.alloc(u32, len);
        @memcpy(report[i], levels_buffer[0..len]);
        i += 1;
    }
}

fn errorIndex(levels: []const u32) ?usize {
    var sign: i8 = 0;
    for (1..levels.len) |i| {
        var diff: u32 = 0;
        var new_sign: i8 = 0;

        if (levels[i - 1] > levels[i]) {
            new_sign = -1;
            diff = levels[i - 1] - levels[i];
        } else {
            new_sign = 1;
            diff = levels[i] - levels[i - 1];
        }
        if (sign == 0) sign = new_sign;

        if ((new_sign != sign) or (diff == 0) or (diff > 3)) {
            return i;
        }
    }
    return null;
}

fn copyWithoutIndex(buffer: []u32, levels: []const u32, index: usize) void {
    @memcpy(buffer[0..index], levels[0..index]);
    @memcpy(buffer[index..(levels.len - 1)], levels[(index + 1)..]);
}

fn isSafe(levels: []const u32, tolerance: Tolerance) bool {
    var error_index: ?usize = null;

    error_index = errorIndex(levels);
    if (error_index == null) {
        return true;
    } else if (tolerance == Tolerance.yes) {
        var buffer: [100]u32 = undefined;
        var new_error_index: ?usize = undefined;
        for (0..levels.len) |index| {
            copyWithoutIndex(buffer[0..], levels, index);
            new_error_index = errorIndex(buffer[0..(levels.len - 1)]);

            if (new_error_index == null) {
                return true;
            }
        }
    }
    return false;
}

fn countSafe(report: [][]const u32, tolerance: Tolerance) u32 {
    var count: u32 = 0;
    for (report) |levels| {
        if (isSafe(levels, tolerance)) {
            count += 1;
        }
    }
    return count;
}

fn splitLine(input: []const u8, buffer: []u32) !usize {
    var iter = std.mem.splitAny(u8, input, " ");
    var index: usize = 0;
    while (iter.next()) |v| {
        buffer[index] = try std.fmt.parseInt(u32, v, 10);
        index += 1;
    }
    return index;
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    var arena = std.heap.ArenaAllocator.init(allocator);
    defer arena.deinit();

    var report: [1000][]u32 = undefined;
    try loadReport(arena.allocator(), &report, "input/02");
    var safe: u32 = undefined;
    safe = countSafe(&report, Tolerance.no);
    std.debug.print("Safe levels: {d}\n", .{safe});
    safe = countSafe(&report, Tolerance.yes);
    std.debug.print("Safe levels with tolerance: {d}\n", .{safe});
}

test "part 1" {
    var report = [_][]const u32{
        &.{ 7, 6, 4, 2, 1 },
        &.{ 1, 2, 7, 8, 9 },
        &.{ 9, 7, 6, 2, 1 },
        &.{ 1, 3, 2, 4, 5 },
        &.{ 8, 6, 4, 4, 1 },
        &.{ 1, 3, 6, 7, 9 },
    };
    const result = countSafe(&report, Tolerance.no);
    try expect(result == 2);
}

test "part 2" {
    var report = [_][]const u32{
        &.{ 7, 6, 4, 2, 1 },
        &.{ 1, 2, 7, 8, 9 },
        &.{ 9, 7, 6, 2, 1 },
        &.{ 1, 3, 2, 4, 5 },
        &.{ 8, 6, 4, 4, 1 },
        &.{ 1, 3, 6, 7, 9 },
    };
    const result = countSafe(&report, Tolerance.yes);
    try expect(result == 4);
}

test "splitLine" {
    const input = "7 6 4 2 1";
    const expected = [_]u32{ 7, 6, 4, 2, 1 };
    var result: [5]u32 = undefined;
    const count = try splitLine(input, &result);

    try expect(5 == count);
    try expect(std.mem.eql(u32, &expected, &result));
}

test "isSafe" {
    try expect(isSafe(&[_]u32{ 7, 6, 4, 2, 1 }, Tolerance.no));
    try expect(!isSafe(&[_]u32{ 1, 2, 7, 8, 9 }, Tolerance.no));
    try expect(!isSafe(&[_]u32{ 9, 7, 6, 2, 1 }, Tolerance.no));
    try expect(!isSafe(&[_]u32{ 1, 3, 2, 4, 5 }, Tolerance.no));
    try expect(!isSafe(&[_]u32{ 8, 6, 4, 4, 1 }, Tolerance.no));
    try expect(isSafe(&[_]u32{ 1, 3, 6, 7, 9 }, Tolerance.no));
}

test "isSafe with tolerance" {
    try expect(isSafe(&[_]u32{ 7, 6, 4, 2, 1 }, Tolerance.yes));
    try expect(!isSafe(&[_]u32{ 1, 2, 7, 8, 9 }, Tolerance.yes));
    try expect(!isSafe(&[_]u32{ 9, 7, 6, 2, 1 }, Tolerance.yes));
    try expect(isSafe(&[_]u32{ 1, 3, 2, 4, 5 }, Tolerance.yes));
    try expect(isSafe(&[_]u32{ 8, 6, 4, 4, 1 }, Tolerance.yes));
    try expect(isSafe(&[_]u32{ 1, 3, 6, 7, 9 }, Tolerance.yes));
    try expect(isSafe(&[_]u32{ 5, 3, 6, 7, 9 }, Tolerance.yes));
}
