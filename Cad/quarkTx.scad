//
// QuarkTx openScad blue print for 3D printer
// (c) 2016-2018 Thierry Wilmot
// All measures are millimeters
//

showComponents = true;
showFramework = true;
showBottom = false;
haveScreen = true;
haveBatteryPanel = true;

boxLenX = 150;
boxLenY = (haveScreen==true) ? 134 : 105;
boxLenZ = 55;
boxThickness = 2;
renfortThickness = 1.2;
boxRound = 12;

bottomPosZ = 10;

switchLenX = 7;
switchLenY = 12.5;
switchLenZ = 9.5;
switchRadius = 3.3;
switchBorder = 12;
switchMainPosY = -46;
switchMainPosZ = 15;
switchMainRadius = 5;

// Color 320x240 Touchscreen, 2.8 inch, ILI9341 Controller
touchScreenLenX = 86;
touchScreenLenY = 50;
touchScreenLenZ = 8;
touchScreenPosY = 6;
screenLenX = 69.2;

screwPlateLenX = 8;
screwPlateLenY = 16;
screwPlateLenZ = 4;
screwPlateHoleRadius = 1;

// Gimbal 62x50x35  x,y,z
gimbalRadius = 47/2;
gimbalPosX = boxLenX/2 - 2*gimbalRadius+10;//38;
gimbalPosY = -20;
gimbalPosZ = -5;
gimbalStickLenZ = 35;
gimbalLenX = 64;
gimbalLenY = 54;
gimbalLenZ = 35;
gimbalSupportX = 46;
gimbalSupportLenZ = 3;  

// Lipo 1000mA/h  Pack Size: 72 x 34 x 14 mm
batteryLenX = 76;
batteryLenY = 16;
batteryLenZ = 35;
batteryPosZ = (haveScreen==true) ? (touchScreenLenZ/2+boxThickness)-5 : 0;

batteryPanelY = boxLenY/2 - batteryLenY - 4;

MotherBoardLenX = 111;
MotherBoardLenY = 10;
MotherBoardLenZ = 39;
MotherBoardPosX = boxLenX/2 - MotherBoardLenX/2 - boxThickness;
MotherBoardPosZ = 4;
MotherBoardSupportLenY = 3;

// TU2  55x26x10
RFModuleLenX = 55;
RFModuleLenY = 11;
RFModuleLenZ = 26;
RFModulePosZ = 6;
RFModuleSupportLenY = 1.5;

// USB 
USBLenX = 10;
USBLenY = 4;
USBPosY = 8;

// Antena
antenaRadius = 3;

gripUpLenZ = 6;

$fn = 90;

//
// Main drawing
//
if(showFramework == true)
{
    union() 
    {
        difference()
        {
            // Main box
            roundCube([boxLenX, boxLenY, boxLenZ], boxRound, center=true);
            translate([0,0,boxThickness])
                roundCube([boxLenX-2*boxThickness, boxLenY-2*boxThickness, boxLenZ], boxRound, center=true);
         
            // Gimbals hole
            translate([-gimbalPosX, gimbalPosY, -boxLenZ/2-boxThickness])
                cylinder(boxThickness*3, gimbalRadius, gimbalRadius);
            translate([gimbalPosX, gimbalPosY, -boxLenZ/2-boxThickness])
                cylinder(boxThickness*3, gimbalRadius, gimbalRadius);
         
            // Switches
            translate([-boxLenX/2+switchBorder, -boxLenY/2+switchBorder, -boxLenZ/2-boxThickness])
                cylinder(boxThickness*3, switchRadius, switchRadius);
            translate([boxLenX/2-switchBorder, -boxLenY/2+switchBorder, -boxLenZ/2-boxThickness])
                cylinder(boxThickness*3, switchRadius, switchRadius);
            translate([-boxLenX/2+switchBorder*2, -boxLenY/2+switchBorder, -boxLenZ/2-boxThickness])
                cylinder(boxThickness*3, switchRadius, switchRadius);
            translate([boxLenX/2-switchBorder*2, -boxLenY/2+switchBorder, -boxLenZ/2-boxThickness])
                cylinder(boxThickness*3, switchRadius, switchRadius);

            translate([0, switchMainPosY+touchScreenLenY/2, -boxLenZ/2-boxThickness])
                cylinder(boxThickness*3, switchMainRadius, switchMainRadius);

            // Antena hole
            translate([0, -boxLenY/2+boxThickness*2, -boxLenZ/4])
                rotate([90, 0, 0])
                    cylinder(boxThickness*3, antenaRadius, antenaRadius);

            // Screen hole
            if(haveScreen == true)
            {
                translate([0, boxLenY/2-touchScreenLenY/2-touchScreenPosY, -boxLenZ/2+boxThickness])
                    cube([screenLenX, touchScreenLenY ,boxThickness*3], center=true);
            }
            
            // USB hole
            translate([boxLenX/2-boxThickness/2, batteryPanelY-MotherBoardSupportLenY-USBPosY, MotherBoardLenZ/4-MotherBoardPosZ])
                cube([5, 3, 5], center=true);
            
            // logo 1
            translate([0, -boxLenY/2+20, -boxLenZ/2])
            {
                translate([0, -9, 0])
                    cube([10, 8.4, 1], center=true);
                
                rotate([180, 0, 0])
                {
                    text("QuarkTx", valign="center", halign="center", size=4);
                }
            }
        }
        
        // logo 2
        translate([0, -boxLenY/2+11, -boxLenZ/2+1])
            rotate([180, 0, 0])
                scale([0.05, 0.05, 0.01])
                    surface(file = "front.png", center = true);
        
        color("YellowGreen")
        {
            // gimBal renforts
            translate([-gimbalPosX, gimbalPosY, -boxLenZ/2+boxThickness/2])
                cylinderRenfort(gimbalRadius, 2);

            translate([gimbalPosX, gimbalPosY, -boxLenZ/2+boxThickness/2])
                cylinderRenfort(gimbalRadius, 2);
            
            // main switch renfort
            translate([0, switchMainPosY+touchScreenLenY/2, -boxLenZ/2+boxThickness/2])
                cylinderRenfort(switchMainRadius, 2);
            
            /*if(haveScreen == true)
            {
                translate([0, boxLenY/2-touchScreenLenY/2-touchScreenPosY, -boxLenZ/2+boxThickness])
                    cubeRenfort([touchScreenLenX, touchScreenLenY, 0], boxThickness);
            }*/
            
            // RFModule support
            translate([0, -boxLenY/2+RFModuleSupportLenY+boxThickness, RFModulePosZ])
            {
                MotherBoardLeg([ RFModuleLenX/2-3, 0,  RFModuleLenY-3], RFModuleSupportLenY);
                MotherBoardLeg([-RFModuleLenX/2+3, 0,  RFModuleLenY-3], RFModuleSupportLenY);
            }

            if(haveBatteryPanel == true)
            {
                // Battery panel
                difference()
                {
                    translate([0, batteryPanelY, 0])
                         cube([boxLenX-boxThickness, boxThickness, boxLenZ-boxThickness*2], center=true);
                    
                    if(haveScreen == true)
                    {
                        translate([0, batteryPanelY, -boxLenZ/2+touchScreenLenZ/2+boxThickness])
                            cube([touchScreenLenX+1, boxThickness+1, touchScreenLenZ+boxThickness+1], center=true);
                    }
                    
                    // for wires
                    translate([-boxLenX/2+16, batteryPanelY+boxThickness, boxLenZ/2-2])
                        rotate([90, 0, 0])
                            cylinder(boxThickness*3, 6, 6);
                }
                
                // MotherBoard support
                translate([MotherBoardPosX, 0, 0])
                {
                    MotherBoardLeg([ MotherBoardLenX/2-MotherBoardSupportLenY, batteryPanelY,  MotherBoardLenZ/2+1], MotherBoardSupportLenY);
                    MotherBoardLeg([ MotherBoardLenX/2-MotherBoardSupportLenY, batteryPanelY, -MotherBoardLenZ/2+6], MotherBoardSupportLenY);
                    MotherBoardLeg([-MotherBoardLenX/2+MotherBoardSupportLenY, batteryPanelY,  MotherBoardLenZ/2+1], MotherBoardSupportLenY);
                    MotherBoardLeg([-MotherBoardLenX/2+MotherBoardSupportLenY, batteryPanelY, -MotherBoardLenZ/2+6], MotherBoardSupportLenY);
                }
            }
        
            // Gimbal support
            translate([-gimbalPosX+gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);
            translate([-gimbalPosX-gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);
            translate([-gimbalPosX-gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);
             translate([-gimbalPosX+gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);

            translate([gimbalPosX+gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);
            translate([gimbalPosX-gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);
            translate([gimbalPosX-gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);
             translate([gimbalPosX+gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
                rotate([90, 0, 0])
                    MotherBoardLeg([0, 0, 0], 4);

            // Screwing parts for bottom
            translate([boxLenX/2-screwPlateLenX/2-boxThickness, -35, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
            translate([boxLenX/2-screwPlateLenX/2-boxThickness, 29, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
            translate([-boxLenX/2+screwPlateLenX/2+boxThickness, -35, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                rotate([0, 180, 0])
                    screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
            translate([-boxLenX/2+screwPlateLenX/2+boxThickness, 29, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                rotate([0, 180, 0])
                    screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);

            translate([50, boxLenY/2-screwPlateLenX/2-boxThickness, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                rotate([0, 0, 90])
                    screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
            translate([-50, boxLenY/2-screwPlateLenX/2-boxThickness, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                rotate([0, 0, 90])
                    screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
            translate([50, -boxLenY/2+screwPlateLenX/2+boxThickness, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                rotate([0, 0, -90])
                    screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
            translate([-50, -boxLenY/2+screwPlateLenX/2+boxThickness, boxLenZ/2-screwPlateLenZ/2-boxThickness])
                rotate([0, 0, -90])
                    screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
        }
        
        // Side renforts
        translate([-boxLenX/2+3, 0, 0])
        {
            difference()
            {
                scale([0.5, 3, 1.3])
                    sphere(20, center=true);
                
                translate([25/2-boxThickness/2, 0, 0])
                    cube([25, 120, 60], center=true);
            }
        }
        translate([boxLenX/2-3, 0, 0])
        {
            difference()
            {
                scale([0.5, 3, 1.3])
                    sphere(20, center=true);
                
                translate([-25/2+boxThickness/2, 0, 0])
                    cube([25, 120, 60], center=true);
                
                translate([10, batteryPanelY-MotherBoardSupportLenY-USBPosY, 6])
                    cube([20, 7, 9], center=true);
            }
        }
    }

    //
    // bottom part
    // 
        
    if(showBottom == true)
    {
        translate([0, 0, bottomPosZ])
        {
            union()
            {
                 // Bottom
                difference()
                {
                    translate([0, 0, boxLenZ/2-boxThickness/2])
                        roundCube([boxLenX-boxThickness*2-0.1, boxLenY-boxThickness*2-0.1, boxThickness], boxRound, center=true);
                    
                    // screw holes
                    translate([boxLenX/2-screwPlateLenX/2-boxThickness-0.7, -35, boxLenZ/2-boxThickness])
                    {
                        cylinder(boxThickness+3, screwPlateHoleRadius+0.2, screwPlateHoleRadius+0.2, center=true);
                        
                        translate([0, 0, 1.8])
                            cylinder(3, 0, 3, center=true);
                    }
                    translate([boxLenX/2-screwPlateLenX/2-boxThickness-0.7, 29, boxLenZ/2-boxThickness])
                    {
                        cylinder(boxThickness+3, screwPlateHoleRadius+0.1, screwPlateHoleRadius+0.1, center=true);
                        
                        translate([0, 0, 1.8])
                            cylinder(3, 0, 3, center=true);
                    }
                    translate([-boxLenX/2+screwPlateLenX/2+boxThickness+0.7, -35, boxLenZ/2-boxThickness])
                    {
                        cylinder(boxThickness+3, screwPlateHoleRadius+0.1, screwPlateHoleRadius+0.1, center=true);
                        
                        translate([0, 0, 1.8])
                            cylinder(3, 0, 3, center=true);
                    }
                    translate([-boxLenX/2+screwPlateLenX/2+boxThickness+0.7, 29, boxLenZ/2-boxThickness])
                    {
                        cylinder(boxThickness+3, screwPlateHoleRadius+0.1, screwPlateHoleRadius+0.1, center=true);
                        
                        translate([0, 0, 1.8])
                            cylinder(3, 0, 3, center=true);
                    }           
                }

                // handle left
                translate([40, 0, boxLenZ-32.1])
                {
                    difference()
                    {
                        scale([1, 2, 0.5])
                            sphere(20, center=true);
                        
                        translate([0, 0, -6])
                            cube([40, 80, 21], center=true);
                    }
                }

                // handle right
                translate([-40, 0, boxLenZ-32.1])
                {
                    difference()
                    {
                        scale([1, 2, 0.5])
                            sphere(20, center=true);
                        
                        translate([0, 0, -6])
                            cube([40, 80, 21], center=true);
                    }
                }
            }
        }
    }

}

//
//  Components inside radio box
//

if(showComponents == true)
{
    // Battery
    color([0.5, 0.5, 0.9], 0.5)
        translate([0, boxLenY/2-batteryLenY/2-boxThickness, batteryPosZ])
            cube([batteryLenX, batteryLenY, batteryLenZ], center=true);

    color([0.5, 0.5, 0.5, 0.7])
    {
        // Gimbal left
        translate([gimbalPosX, gimbalPosY, gimbalPosZ-boxThickness]) 
        {
            cube([gimbalLenX, gimbalLenY, gimbalLenZ], center=true);
            
            // Stick
            translate([0, 0, -gimbalStickLenZ-gimbalLenZ/2])
                cylinder(gimbalStickLenZ, 3, 2);
        }
        
        // Gimbal rigth
        translate([-gimbalPosX, gimbalPosY, gimbalPosZ-boxThickness])
        {
            cube([gimbalLenX, gimbalLenY, gimbalLenZ], center=true);
            
            // Stick
            translate([0, 0, -gimbalStickLenZ-gimbalLenZ/2])
                cylinder(gimbalStickLenZ, 3, 2);
        }

        // Screen
        if(haveScreen == true)
        {
            translate([0, boxLenY/2-touchScreenLenY/2-touchScreenPosY, -boxLenZ/2+boxThickness+touchScreenLenZ/2])
                cube([touchScreenLenX, touchScreenLenY, touchScreenLenZ], center=true);
        }

        // Switches
        switch([-boxLenX/2+switchBorder,   -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);
        switch([ boxLenX/2-switchBorder,   -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);
        switch([-boxLenX/2+switchBorder*2, -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);
        switch([ boxLenX/2-switchBorder*2, -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);

        // main switch
        translate([0, switchMainPosY+touchScreenLenY/2, -boxLenZ/2-boxThickness])
            cylinder(switchMainPosZ, switchMainRadius, switchMainRadius);
    }
    
    color("DarkSlateGray")
    {
        // Antena
        translate([0, -boxLenY/2+5, -boxLenZ/4])
        {
            rotate([90, 0, 0])
            {
                union() 
                {
                    cylinder(15, 3, 3);
                    translate([0.2, 0, 14])
                    {
                        rotate([10, -20, 0])
                        {
                            cylinder(30, 3, 3);
                            translate([0, 0, 30])
                                sphere(3);
                        }
                    }
                }
            }
        }
    }
  
    // MotherBoard
    if(haveBatteryPanel == true)
    {
        color([0.8, 0.5, 0.5], 0.6)
            translate([MotherBoardPosX, batteryPanelY-MotherBoardLenY/2-MotherBoardSupportLenY, MotherBoardPosZ])
                cube([MotherBoardLenX, MotherBoardLenY, MotherBoardLenZ], center=true);
    }
    
    // RFModule
    color([0.8, 0.5, 0.5], 0.6)
    {
        translate([0, -boxLenY/2+RFModuleLenY/2+RFModuleSupportLenY+boxThickness, RFModulePosZ])
        {
            cube([RFModuleLenX, RFModuleLenY, RFModuleLenZ], center=true);
        }
    }
}

module switch(v)
{
    translate([v[0], v[1], v[2]])
    {
        roundCube([switchLenX, switchLenY, switchLenZ], 0.5, center=true);
        
        translate([0, -1.3, -14])
            rotate([-10, 0, 0])
                cylinder(10, 1, 0.5);
    }
}

module roundCube(v, r, center)
{
    hull()
    {
        translate([ v[0]/2-r,  v[1]/2-r, 0]) cylinder(v[2], r, r, center);
        translate([-v[0]/2+r,  v[1]/2-r, 0]) cylinder(v[2], r, r, center);
        translate([-v[0]/2+r, -v[1]/2+r, 0]) cylinder(v[2], r, r, center);
        translate([ v[0]/2-r, -v[1]/2+r, 0]) cylinder(v[2], r, r, center);
    }
}

module screwPlate(v, r, center)
{
    union() 
    {
        difference()
        {
            cube([v[0], v[1], v[2]], center);
            
            translate([-v[0]/2, 0, 0])
                cube([3, v[1], v[2]+1], center);
            
            translate([-0.7, 0, 0])
                cylinder(v[2]+1, r, r, center);
            
            translate([-v[0]/2+2, -v[1]/2, 0])
                cylinder(v[2]+0.5, 6, 6, center);
            
            translate([-v[0]/2+2, v[1]/2, 0])
                cylinder(v[2]+0.5, 6, 6, center);
            
        }
        
        translate([-2.44, 0, 0])
            roundCube([0.5, 4.04, v[2]], 0.2, center);
    }
}

module cylinderRenfort(radius, thickness)
{
    difference()
    {
        cylinder(renfortThickness, radius+thickness, radius+thickness);

        translate([0, 0, -2])
            cylinder(boxThickness*3, radius, radius);
    }
}

module cubeRenfort(v, thickness)
{
    difference()
    {
        cube([v[0]+thickness*2, v[1]+thickness*2, v[2]+thickness*2], center=true);
        
    }
}

module MotherBoardLeg(v, h)
{
    difference()
    {
        translate(v)
            rotate([90, 0, 0])
                cylinder(h, 2, 2);
        
        translate([v[0], v[1]+0.5, v[2]])
            rotate([90, 0, 0])
                cylinder(h+1, 1, 1);
    }
}

