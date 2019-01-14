/* Objective function */
max: -C1 +3C2 -3C3;

/* Constraints */
+3C1 -C2 -2C3 <= 7;
-2C1 -4C2 +4C3 <= 3;
+C1 -2C3 <= 4;
-2C1 +2C2 +C3 <= 8;
+3C1 <= 5;

/* Integer definitions */
int C1,C2,C3;