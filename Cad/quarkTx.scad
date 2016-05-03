//
// QuarkTx openScad blue print for 3D printer
// (c) 2016 Thierry Wilmot
// All measures are in millimeters
//

showComponents = true;
showBottom = true;
haveScreen = false;
haveBatteryPanel = true;

boxLenX = 150;
boxLenY = 102;
boxLenZ = 42;
boxThickness = 2;
boxRound = 12;

bottomPosZ = 13;

switchLenX = 7;
switchLenY = 12.5;
switchLenZ = 9.5;
switchRadius = 3.3;
switchBorder = 12;
switchMainPosY = -30;
switchMainPosZ = 15;
switchMainRadius = 4;

screenLenX = 86;
screenLenY = 50;
screenLenZ = 8;
screenPosY = 6;

screwPlateLenX = 6;
screwPlateLenY = 16;
screwPlateLenZ = 3;
screwPlateHoleRadius = 1.5;

// Gimbal 62x50x35  x,y,z
gimbalRadius = 47/2;
gimbalPosX = 38;
gimbalPosY = -5;
gimbalPosZ = 1;
gimbalStickLenZ = 25;
gimbalLenX = 64;
gimbalLenY = 54;
gimbalLenZ = 35;
gimbalSupportX = 46; // Todo ajouter les supports
gimbalSupportLenZ = 3;  

// Lipo 1000mA/h  Pack Size: 72 x 34 x 14mm
batteryLenX = 76;
batteryLenY = 16;
batteryLenZ = 35;
BatteryPosZ = 0;

batteryPanelY = boxLenY/2 - batteryLenY - 4;

PCBLenX = 110;
PCBLenY = 1;
PCBLenZ = 30;
PCBPosX = boxLenX/2 - PCBLenX/2 - boxThickness;
PCBPosZ = 4;
PCBSupportLenY = 3;

// TU2  55x26x10
RFModuleLenX = 55;
RFModuleLenY = 11;
RFModuleLenZ = 26;
RFModulePosZ = 6;
RFModuleSupportLenY = 1.5;

// USB 
USBLenX = 10;
USBLenY = 4;

// Antena
antenaRadius = 3;

gripUpLenZ = 6;

$fn = 190;

//
// Main drawing
//

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

        translate([0, switchMainPosY+screenLenY/2, -boxLenZ/2-boxThickness])
            cylinder(boxThickness*3, switchMainRadius, switchMainRadius);

        // Antena hole
        translate([0, -boxLenY/2+boxThickness*2, -boxLenZ/4])
            rotate([90, 0, 0])
                cylinder(boxThickness*3, antenaRadius, antenaRadius);

        // Screen hole
        if(haveScreen == true)
        {
            translate([0, boxLenY/2-screenLenY/2-screenPosY, -boxLenZ/2+boxThickness])
                cube([screenLenX, screenLenY ,boxThickness*3], center=true);
        }
        
        // USB hole
        translate([boxLenX/2-boxThickness/2, batteryPanelY-PCBSupportLenY-2, PCBLenZ/4-PCBPosZ])
            cube([5, 3, 5], center=true);
    }

    color([0.64, 0.8, 0.4])
    {
        // gimBal renforts
        translate([-gimbalPosX, gimbalPosY, -boxLenZ/2+boxThickness/2])
            cylinderRenfort(gimbalRadius, 3);

        translate([gimbalPosX, gimbalPosY, -boxLenZ/2+boxThickness/2])
            cylinderRenfort(gimbalRadius, 3);
        
        translate([0, switchMainPosY+screenLenY/2, -boxLenZ/2+boxThickness/2])
            cylinderRenfort(switchMainRadius, 3);
        
        if(haveScreen == true)
        {
            //translate([0, boxLenY/2-screenLenY/2-screenPosY, -boxLenZ/2+boxThickness])
            //    cubeRenfort([screenLenX, screenLenY, 0], boxThickness);
        }
        
        // RFModule support
        translate([0, -boxLenY/2+RFModuleSupportLenY+boxThickness, RFModulePosZ])
        {
            PCBLeg([ RFModuleLenX/2-3, 0,  RFModuleLenY-3], RFModuleSupportLenY);
            PCBLeg([-RFModuleLenX/2+3, 0,  RFModuleLenY-3], RFModuleSupportLenY);
        }
    }

    if(haveBatteryPanel == true)
    {
        color([0.64, 0.8, 0.4])
        {
            // Battery panel
            difference()
            {
                translate([0, batteryPanelY, 0])
                     cube([boxLenX-boxThickness, boxThickness, boxLenZ-boxThickness*2], center=true);
                
                if(haveScreen == true)
                {
                    translate([0, batteryPanelY, -boxLenZ/2+screenLenZ/2+boxThickness])
                        cube([screenLenX, boxThickness+1, screenLenZ+boxThickness], center=true);
                }
                
                translate([-boxLenX/2+16, batteryPanelY+boxThickness, boxLenZ/2-2])
                    rotate([90, 0, 0])
                        cylinder(boxThickness*3, 6, 6);
            }
            
            // PCB support
            translate([PCBPosX, 0, 0])
            {
                PCBLeg([ PCBLenX/2-3, batteryPanelY,  PCBLenZ/2+1], PCBSupportLenY);
                PCBLeg([ PCBLenX/2-3, batteryPanelY, -PCBLenZ/2+6], PCBSupportLenY);
                PCBLeg([-PCBLenX/2+3, batteryPanelY,  PCBLenZ/2+1], PCBSupportLenY);
                PCBLeg([-PCBLenX/2+3, batteryPanelY, -PCBLenZ/2+6], PCBSupportLenY);
            }
        }
    }
    
    // Gimbal support
    color([0.64, 0.8, 0.4])
    {
        translate([-gimbalPosX+gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);
        translate([-gimbalPosX-gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);
        translate([-gimbalPosX-gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);
         translate([-gimbalPosX+gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);

        translate([gimbalPosX+gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);
        translate([gimbalPosX-gimbalSupportX/2, gimbalPosY+gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);
        translate([gimbalPosX-gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);
         translate([gimbalPosX+gimbalSupportX/2, gimbalPosY-gimbalSupportX/2, -boxLenZ/2+boxThickness/2+4])
            rotate([90, 0, 0])
                PCBLeg([0, 0, 0], 4);
    }

    // Screwing parts
    translate([boxLenX/2-screwPlateLenX/2-boxThickness, -25, boxLenZ/2-screwPlateLenZ/2-boxThickness])
        screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
    translate([boxLenX/2-screwPlateLenX/2-boxThickness, 18, boxLenZ/2-screwPlateLenZ/2-boxThickness])
        screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
    translate([-boxLenX/2+screwPlateLenX/2+boxThickness, -25, boxLenZ/2-screwPlateLenZ/2-boxThickness])
        rotate([0, 180, 0])
            screwPlate([screwPlateLenX, screwPlateLenY, screwPlateLenZ], screwPlateHoleRadius, center=true);
    translate([-boxLenX/2+screwPlateLenX/2+boxThickness, 18, boxLenZ/2-screwPlateLenZ/2-boxThickness])
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


//
//
// 
    
if(showBottom == true)
{
    translate([0, 0, bottomPosZ])
    {
         // Bottom
        difference()
        {
            translate([0, 0, boxLenZ/2-boxThickness/2])
                roundCube([boxLenX-boxThickness*2-0.1, boxLenY-boxThickness*2-0.1, boxThickness], boxRound, center=true);
            
            // holes
            translate([boxLenX/2-screwPlateLenX/2-boxThickness+0.5, -25, boxLenZ/2-boxThickness])
            {
                cylinder(boxThickness+3, screwPlateHoleRadius, screwPlateHoleRadius, center=true);
                
                translate([0, 0, 1.5])
                    cylinder(3, 0, 3, center=true);
            }
            translate([boxLenX/2-screwPlateLenX/2-boxThickness+0.5, 18, boxLenZ/2-boxThickness])
            {
                cylinder(boxThickness+3, screwPlateHoleRadius, screwPlateHoleRadius, center=true);
                
                translate([0, 0, 1.5])
                    cylinder(3, 0, 3, center=true);
            }
            translate([-boxLenX/2+screwPlateLenX/2+boxThickness-0.5, -25, boxLenZ/2-boxThickness])
            {
                cylinder(boxThickness+3, screwPlateHoleRadius, screwPlateHoleRadius, center=true);
                
                translate([0, 0, 1.5])
                    cylinder(3, 0, 3, center=true);
            }
            translate([-boxLenX/2+screwPlateLenX/2+boxThickness-0.5, 18, boxLenZ/2-boxThickness])
            {
                cylinder(boxThickness+3, screwPlateHoleRadius, screwPlateHoleRadius, center=true);
                
                translate([0, 0, 1.5])
                    cylinder(3, 0, 3, center=true);
            }           
        }

        // handle left
        translate([40, 0, boxLenZ-25.9])
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
        translate([-40, 0, boxLenZ-25.9])
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

//
//  Components inside radio box
//

if(showComponents == true)
{
    // Battery
    color([0.5, 0.5, 0.9], 0.5)
        translate([0, boxLenY/2-batteryLenY/2-boxThickness, BatteryPosZ])
            cube([batteryLenX, batteryLenY, batteryLenZ], center=true);

    color([0.5, 0.5, 0.5], 0.8)
    {
        // Gimbal
        translate([gimbalPosX, gimbalPosY, 0]) 
        {
            cube([gimbalLenX, gimbalLenY, gimbalLenZ], center=true);
            translate([0, 0, -gimbalStickLenZ-gimbalLenZ/2])
                cylinder(gimbalStickLenZ, 2, 1);
        }
        
        translate([-gimbalPosX, gimbalPosY, 0])
        {
            cube([gimbalLenX, gimbalLenY, gimbalLenZ], center=true);
            translate([0, 0, -gimbalStickLenZ-gimbalLenZ/2])
                cylinder(gimbalStickLenZ, 2, 1);
        }

        // Screen
        if(haveScreen == true)
        {
            translate([0, boxLenY/2-screenLenY/2-screenPosY, -boxLenZ/2+boxThickness+screenLenZ/2])
                cube([screenLenX, screenLenY, screenLenZ], center=true);
        }

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

        // Switches
        switch([-boxLenX/2+switchBorder,   -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);
        switch([ boxLenX/2-switchBorder,   -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);
        switch([-boxLenX/2+switchBorder*2, -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);
        switch([ boxLenX/2-switchBorder*2, -boxLenY/2+switchBorder, -boxLenZ/2+switchLenZ/2+boxThickness]);

        // main switch
        translate([0, switchMainPosY+screenLenY/2, -boxLenZ/2-boxThickness])
            cylinder(switchMainPosZ, switchMainRadius, switchMainRadius);
    }
  
    // PCB
    if(haveBatteryPanel == true)
    {
        color([0.8, 0.5, 0.5], 0.8)
            translate([PCBPosX, batteryPanelY-PCBSupportLenY, PCBPosZ])
                cube([PCBLenX, PCBLenY, PCBLenZ], center=true);
    }
    
    // RFModule
    color([0.8, 0.5, 0.5], 0.8)
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
            
            translate([0.5, 0, 0])
                cylinder(v[2]+1, r, r, center);
            
            translate([-v[0]/2+1, -v[1]/2, 0])
                cylinder(v[2]+0.5, 5, 5, center);
            
            translate([-v[0]/2+1, v[1]/2, 0])
                cylinder(v[2]+0.5, 5, 5, center);
            
        }
        
        translate([-1.44, 0, 0])
            roundCube([0.5, 6.05, v[2]], 0.3, center);
    }
}

module cylinderRenfort(radius, thickness)
{
    difference()
    {
        cylinder(boxThickness, radius+thickness, radius+thickness);

        translate([0, 0, -2])
            cylinder(boxThickness*4, radius, radius);
    }
}

module cubeRenfort(v, thickness)
{
    difference()
    {
        cube([v[0]+thickness*2, v[1]+thickness*2, v[2]+thickness*2], center=true);
        
    }
}

module PCBLeg(v, h)
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

