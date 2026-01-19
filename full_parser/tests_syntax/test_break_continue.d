void testLoop() {
    while (true) {
        if (x == 5) {
            break;
        }
        if (x == 3) {
            continue;
        }
        x += 1;
    }
}
