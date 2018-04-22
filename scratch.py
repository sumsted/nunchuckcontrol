X_RANGE = (35, 130, 225)
Y_RANGE = (26, 124, 220)

HIGH = 2
CENTER = 1
LOW = 0


def to_scale(x, y):
    new_x = int((x - X_RANGE[LOW]) / (X_RANGE[HIGH] - X_RANGE[LOW]) * 100)
    new_y = int((y - Y_RANGE[LOW]) / (Y_RANGE[HIGH] - Y_RANGE[LOW]) * 100)
    return new_x, new_y


def to_zero_base(x, y):
    new_x = (x * 2) + -100
    new_y = (y * 2) + -100
    return new_x, new_y


def to_range(x, y):
    new_x = int(x / 10) * 10
    new_y = int(y / 10) * 10
    return new_x, new_y


def to_left_right(x, y):
    left = int(y + (x / 100 * y)) if x < 0 else y
    right = int(y - (x / 100 * y)) if x > 0 else y
    return left, right


def all(start_x, start_y):
    new_x = int((start_x - X_RANGE[LOW]) / (X_RANGE[HIGH] - X_RANGE[LOW]) * 100)
    new_y = int((start_y - Y_RANGE[LOW]) / (Y_RANGE[HIGH] - Y_RANGE[LOW]) * 100)
    new_x = (new_x * 2) + -100
    new_y = (new_y * 2) + -100
    new_x = int(new_x / 10) * 10
    new_y = int(new_y / 10) * 10
    left = int(new_y + (new_x / 100 * new_y)) if new_x < 0 else new_y
    right = int(new_y - (new_x / 100 * new_y)) if new_x > 0 else new_y
    return left, right


def start(start_x, start_y):
    scaled_x, scaled_y = to_scale(start_x, start_y)
    zero_x, zero_y = to_zero_base(scaled_x, scaled_y)
    x, y = to_range(zero_x, zero_y)
    left, right = to_left_right(x, y)
    all_l, all_r = all(start_x, start_y)
    print((start_x, start_y), (scaled_x, scaled_y), (zero_x, zero_y), (x, y), (left, right), (all_l, all_r))


if __name__ == '__main__':
    # X_RANGE = (35, 130, 225)
    # Y_RANGE = (26, 124, 220)
    print("start", "scaled", "zero", "xy", "lr", "alar\n")
    start(84, 100)

    # for x in X_RANGE:
    #     for y in Y_RANGE:
    for x in range(X_RANGE[LOW], X_RANGE[HIGH], 50):
        for y in range(Y_RANGE[LOW], Y_RANGE[HIGH], 50):
            print()
            start(x, y)
