const std = @import("std");

fn loadLists(left: *std.ArrayList(u32), right: *std.ArrayList(u32), path: []const u8) !void {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var buffer: [1024]u8 = undefined;
    const reader = file.reader();
    while (try reader.readUntilDelimiterOrEof(&buffer, '\n')) |line| {
        var iter = std.mem.splitAny(u8, line, " ");
        var list = left;
        while (iter.next()) |entry| {
            if (!std.mem.eql(u8, entry, "")) {
                try list.append(try std.fmt.parseInt(u32, entry, 10));
                list = right;
            }
        }
    }
}

fn computeDistance(left_items: []u32, right_items: []u32) u32 {
    std.mem.sort(u32, left_items, {}, comptime std.sort.asc(u32));
    std.mem.sort(u32, right_items, {}, comptime std.sort.asc(u32));

    const size = left_items.len;
    var sum: u32 = 0;
    for (0..size) |i| {
        const diff = if (left_items[i] > right_items[i]) left_items[i] - right_items[i] else right_items[i] - left_items[i];
        sum += diff;
    }
    return sum;
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    var left_list = std.ArrayList(u32).init(allocator);
    var right_list = std.ArrayList(u32).init(allocator);
    defer left_list.deinit();
    defer right_list.deinit();

    try loadLists(&left_list, &right_list, "./input");

    std.debug.print("Sum: {d}\n", .{computeDistance(left_list.items, right_list.items)});
}

const expect = std.testing.expect;
const expectEqual = std.testing.expectEqual;

test "part 1" {
    var left_items = [_]u32 {3, 4, 2, 1, 3, 3};
    var right_items = [_]u32 {4, 3, 5, 3, 9, 3};

    try expectEqual(11, computeDistance(&left_items, &right_items));
}
