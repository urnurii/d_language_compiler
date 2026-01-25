void main()
{
    // FOR
    int forSum = 0;
    for (int i = 0; i < 10; i += 1)
    {
        if (i == 3) continue;
        if (i == 6) break;
        forSum += i;
    }
    write("FOR_SUM(expected 12)=");
    writeln(forSum);

    int forContinueInc = 0;
    for (int i = 0; i < 5; i += 1)
    {
        if (i == 2) { 
            forContinueInc += 100; 
            continue; 
        }
        forContinueInc += 1;
    }
    write("FOR_CONTINUE_INC(expected 104)=");
    writeln(forContinueInc);

    int forZero = 0;
    for (int i = 0; i < 0; i += 1)
        forZero += 1;
    write("FOR_ZERO_ITERS(expected 0)=");
    writeln(forZero);

    // WHILE
    int w = 5;
    int whileSum = 0;
    while (w >= 0)
    {
        if (w == 4) { w -= 1; continue; }
        if (w == 1) break;                
        whileSum += w;
        w -= 1;
    }
    write("WHILE_SUM(expected 10)=");
    writeln(whileSum);

    int whileZero = 0;
    int wz = 0;
    while (wz < 0)
        whileZero += 1;
    write("WHILE_ZERO_ITERS(expected 0)=");
    writeln(whileZero);

    // DO-WHILE
    int dOnce = 0;
    int dCounter = 0;
    do
    {
        dOnce += 7;
        dCounter += 1;
    } while (false);
    write("DO_ONCE_SUM(expected 7)=");
    writeln(dOnce);     
    write("DO_ONCE_CNT(expected 1)=");
    writeln(dCounter);

    // DO-WHILE
    int d = 0;
    int doSum = 0;
    do
    {
        d += 1;
        if (d == 2) continue;
        if (d == 4) break;    
        doSum += d;           
    } while (d < 10);
    write("DO_SUM(expected 4)=");
    writeln(doSum);

    // FOREACH
    int[] arr = [1, 2, 3, 4, 5];
    int foreachSum = 0;
    foreach (x; arr)
    {
        foreachSum += x;
    }
    write("FOREACH_SUM(expected 15)=");
    writeln(foreachSum);

    // NESTED LOOPS
    int nested = 0;
    for (int i = 0; i < 3; i += 1)
    {
        int j = 0;
        while (j < 5)
        {
            j += 1;
            if (j == 2) continue; 
            if (j == 4) break;    
            nested += 1;          
        }
        nested += 10;             
    }
    write("NESTED(expected 36)=");
    writeln(nested); 


    int[] arr2 = [1, 2, 3, 4, 5];
    int foreachSum2 = 0;
    foreach (x; arr2[1..4])
    {
        foreachSum2 += x;
    }
    write("FOREACH_SUM(expected 9)=");
    writeln(foreachSum2);
}
