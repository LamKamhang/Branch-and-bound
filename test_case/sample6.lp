/* Objective function */
max: +2C1 +3C2 -C3 -12C4;

/* Constraints */
-2C1 -9C2 +C3 +9C4 <= 0;
+C1 +3C2 -C3 -6C4 <= 0;

/* Integer definitions */
int C1,C2,C3,C4;
