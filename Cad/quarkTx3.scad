    
    
$fn = 90;

difference()
{
    mainShape();
    
    scale([.95, .95, 1.5])
        mainShape();
}


module mainShape()
{
    difference()
    {
        translate([0,3,0])
            scale([1,2,0.3])
                sphere (10);
        
        translate([0,20,0])
            cube([20,20,10], true);
        
        translate([0,-20,0])
            cube([20,20,10], true);
    }
}