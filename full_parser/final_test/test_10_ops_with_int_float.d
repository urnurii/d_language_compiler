void main()
{
   int x1 = 5;
   int x2 = 3;
   float y1 = 2.5;
   float y2 = 3.5;

   writeln("INT and INT");
   writeln(x1+x2);
   writeln(x1-x2);
   writeln(x1*x2);
   writeln(x1/x2);
   writeln("\n");

   writeln("INT and FLOAT");
   writeln(x1+y1);
   writeln(x1-y1);
   writeln(x1*y1);
   writeln(x1/y1);
   writeln("\n");

   writeln("FLOAT and FLOAT");
   writeln(y2+y1);
   writeln(y2-y1);
   writeln(y2*y1);
   writeln(y2/y1);
   writeln("\n");
   
   writeln("FLOAT and INT");
   writeln(y2+x1);
   writeln(y2-x1);
   writeln(y2*x1);
   writeln(y2/x1);
}