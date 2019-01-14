/* Objective function */
max: +10C1 -57C2 -9C3 -24C4;

/* Constraints */
+C1 -11C2 -5C3 +18C4 <= 0;
+C1 -3C2 -C3 +2C4 <= 0;
+C1 <= 1;

/* Integer definitions */
int C1,C2,C3,C4;