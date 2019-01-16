/* Objective function */
max: -3C1 +4C2;

/* Constraints */
-4C1 -2C2 <= -8;
-2C1 <= -2;
+3C1 +2C2 <= 10;
-C1 +3C2 <= 1;
-3C2 <= -2;

/* Integer definitions */
int C1,C2;