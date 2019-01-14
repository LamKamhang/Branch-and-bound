/* Objective function */
max: +C1;

/* Constraints */
+C1 +C2 +C5 +C999 = 1;
+C3 +C2 +5C6 -6C999 <= 2;
+C4 +3C5 +C6 >= 3;

R10: C1 <= 150;
C2 >= 3;
R11: C3 = 5;


/* Integer definitions */
int C1,C2,C3,C4, C5, C6, C999;
