const std = @import("std");
const expect = std.testing.expect;

fn loadReport(allocator: std.mem.Allocator, report: [][]u32, path: []const u8) !void {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var buffer: [1024]u8 = undefined;
    const reader = file.reader();
    var i: usize = 0;
    while (try reader.readUntilDelimiterOrEof(&buffer, '\n')) |line| {
        var levels_buffer: [10]u32 = undefined;
        const len = try splitLine(line, &levels_buffer);
        report[i] = try allocator.alloc(u32, len);
        @memcpy(report[i], levels_buffer[0..len]);
        i += 1;
    }
}

fn isSafe(levels: []const u32, ) bool {
    var sign: i8 = 0;
    for (1..levels.len) |i| {
        var diff: u32 = 0;
        if (levels[i] < levels[i-1]) {
            if (sign > 0) return false;

            sign = -1;
            diff = levels[i-1] - levels[i];
        } else {
            if (sign < 0) return false;

            sign = 1;
            diff = levels[i] - levels[i-1];
        }
        if ((diff == 0) or (diff > 3)) {
            return false;
        }
    }
    return true;
}

fn countSafe(report: [][]const u32) u32 {
    var count: u32 = 0;
    for (report) |levels| {
        if (isSafe(levels)) {
            count += 1;
        }
    }
    return count;
}

fn splitLine(input: []const u8, buffer: []u32) !usize {
    var iter = std.mem.splitAny(u8, input, " ");
    var index: usize = 0;
    while(iter.next()) |v| {
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
    const safe = countSafe(&report);
    std.debug.print("Safe levels: {d}\n", .{ safe });
}

test "part 1" {
    var report = [_][]const u32 {
        &.{ 7, 6, 4, 2, 1 },
        &.{ 1, 2, 7, 8, 9 },
        &.{ 9, 7, 6, 2, 1 },
        &.{ 1, 3, 2, 4, 5 },
        &.{ 8, 6, 4, 4, 1 },
        &.{ 1, 3, 6, 7, 9 },
    };
    const result = countSafe(&report);
    try expect(result == 2);
}

test "splitLine" {
    const input = "7 6 4 2 1";
    const expected = [_]u32 { 7, 6, 4, 2, 1 };
    var result: [5]u32 = undefined;
    const count = try splitLine(input, &result);

    try expect(5 == count);
    try expect(std.mem.eql(u32, &expected, &result));
}

test "isSafe" {
    try expect(isSafe(&[_]u32{ 7, 6, 4, 2, 1 }));
    try expect(!isSafe(&[_]u32{ 1, 2, 7, 8, 9 }));
    try expect(!isSafe(&[_]u32{ 9, 7, 6, 2, 1 }));
    try expect(!isSafe(&[_]u32{ 1, 3, 2, 4, 5 }));
    try expect(!isSafe(&[_]u32{ 8, 6, 4, 4, 1 }));
    try expect(isSafe(&[_]u32{ 1, 3, 6, 7, 9 }));
}

