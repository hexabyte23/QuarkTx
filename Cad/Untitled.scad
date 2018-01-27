
$fn = 200;

intersection_for(n = [1 : 3])
{
    rotate([0, 0, n * 90])
    {
        translate([11,0,0])
            sphere(12);
    }
}