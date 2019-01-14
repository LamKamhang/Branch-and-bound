/* Objective function */
max: +2C1 -C2 +C3;

/* Constraints */
+2C1 +C2 -2C3 +C4 -5C5<= 8;
+4C1 -C2 +2C3 +2C4 <= 2;
+2C1 +3C2 -C3 +C5 >= 4;

/* Integer definitions */
int C1,C2,C3,C4,C5;