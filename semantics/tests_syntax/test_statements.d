void testStatements() {
    if (x > 0) {
        y = 1;
    }
    
    if (a < b) {
        x = 1;
    } else {
        x = 2;
    }
    
    while (i < 10) {
        i += 1;
    }
    
    do {
        x += 1;
    } while (x < 10);
    
    for (int i = 0; i < 10; i+=1) {
        sum += i;
    }
    
    for (;;) {
        break;
    }
}
