//
// QuarkTx openScad blue print for 3D printer
// (c) 2016 Thierry Wilmot
// All measures are in millimeters
//

boxLenX = 100;
boxLenY = 70;
boxLenZ = 20;
boxThickness = 0.5;

gimballRadius = 11;

screenLenX = 60;
screenLenY = 23;

$fs = 0.01;

difference()
{
    // main cube
    cube([boxLenX,boxLenY,boxLenZ], center=true);
    translate([0,0,boxThickness])
        cube([boxLenX-2*boxThickness,boxLenY-2*boxThickness,boxLenZ], center=true);
    
    // gimbals print
    translate([-25,-10,-boxLenZ/2-boxThickness])
        cylinder(boxThickness*3, gimballRadius, gimballRadius);
    translate([25,-10,-boxLenZ/2-boxThickness])
        cylinder(boxThickness*3, gimballRadius, gimballRadius);
    
    // switches
    translate([-40,-27,-boxLenZ/2-boxThickness])
        cylinder(boxThickness*3, 2, 2);
    translate([40,-27,-boxLenZ/2-boxThickness])
        cylinder(boxThickness*3, 2, 2);
    translate([-33,-27,-boxLenZ/2-boxThickness])
        cylinder(boxThickness*3, 2, 2);
    translate([33,-27,-boxLenZ/2-boxThickness])
        cylinder(boxThickness*3, 2, 2);
    
    translate([0,-20+screenLenY/2,-boxLenZ/2+boxThickness])
        cube([4, 6, boxThickness*3], center=true);
    
    // antena print
    translate([0,-boxLenY/2+boxThickness*2,0])
        rotate([90,0,0])
            cylinder(boxThickness*3, 3, 3);
    
    // screen print
    translate([0,7+screenLenY/2,-boxLenZ/2+boxThickness])
        cube([screenLenX, screenLenY ,boxThickness*3], center=true);
};

