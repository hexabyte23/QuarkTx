
showFront = false;
showBack = true;

boxLenX = 150;
boxLenY = 134;
boxLenZ = 55;
boxThickness = 6;
radiusCorner = 15;


$fn = 80;

/*
union()
{
    difference()
    {
        roundFrame([boxLenX, boxLenY, boxLenZ], radiusCorner , center=true);
        
        roundFrame([boxLenX-boxThickness, boxLenY-boxThickness, boxLenZ+2], radiusCorner , center=true);
        
        translate([0, 0, boxLenZ/2])
            roundCube([boxLenX-boxThickness/2, boxLenY-boxThickness/2, 2], radiusCorner , center=true);
        
        translate([0, 0, -boxLenZ/2])
            roundCube([boxLenX-boxThickness/2, boxLenY-boxThickness/2, 2], radiusCorner , center=true);
    }
}

if(showFront)
{
 
    color([0.6,0.6,0.6])
         translate([0, 0, boxLenZ/2-1])
            roundCube([boxLenX-boxThickness/2, boxLenY-boxThickness/2, 2], radiusCorner , center=true);   
    
}

pa = 5;

module roundFrame(v, r, center)
{
    //hull()
    {
        translate([ v[0]/2-r-pa,  v[1]/2-r, 0]) cylinder(v[2], r, r, center);
        translate([-v[0]/2+r+pa,  v[1]/2-r, 0]) cylinder(v[2], r, r, center);
        translate([-v[0]/2+r, -v[1]/2+r, 0]) cylinder(v[2], r, r, center);
        translate([ v[0]/2-r, -v[1]/2+r, 0]) cylinder(v[2], r, r, center);
        translate([ 0, v[1]/2-60, 0]) cylinderPart(v[2], 70);
       
        translate([v[0]/2-6, 0, 0])
        {
            rotate([0,0,2])
                scale([0.5, 2.6, 1.2])
                    sphere(20);
        }
               
        translate([-v[0]/2+6, 0, 0])
        {
            rotate([0,0,-2])
                scale([0.5, 2.8, 1.2])
                    sphere(20);
        }
    }
}
*/

cylinderPart(55,100, 45);

module cylinderPart(height, radius, angle)
{
    difference()
    {
        cylinder(height, radius, radius, center=true);
        
        translate([0, -radius/2, 0])
            cube([200, radius, height+1], center=true);
        
        rotate([0,0,angle])
            translate([0, -radius/2, 0])
                cube([200, radius, height+1], center=true);
        
        rotate([0,0,-angle])
            translate([0, -radius/2, 0])
                cube([200, radius, height+1], center=true);
    }
}


module roundCube(v, r, center)
{
    hull()
    {
        translate([ v[0]/2-r-pa,  v[1]/2-r, 0]) cylinder(v[2], r, r, center);
        translate([-v[0]/2+r+pa,  v[1]/2-r, 0]) cylinder(v[2], r, r, center);
        translate([-v[0]/2+r, -v[1]/2+r, 0]) cylinder(v[2], r, r, center);
        translate([ v[0]/2-r, -v[1]/2+r, 0]) cylinder(v[2], r, r, center);
    }
}

