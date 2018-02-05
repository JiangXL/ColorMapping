<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;
    $defaultcamid = isset($_GET['camid'])?$_GET['camid']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT UserLevel,DefaultPlannerName,DefaultGearName FROM user WHERE(UserName=$username AND PassWord=$password)");
    if($row = mysql_fetch_array($result))
    {
        $userleve = $row['UserLevel'];
        $defaultplannername = $row['DefaultPlannerName'];
        $defaultgearname = $row['DefaultGearName'];

        mysql_query("UPDATE user set DefaultCamId=$defaultcamid WHERE(UserName=$username AND PassWord=$password)"); 
        
        echo "RetVal="."0".","."UserLevel=".$userleve.","."DefaultPlannerName=".$defaultplannername.","."DefaultGearName=".$defaultgearname;
    }
    else
    {
        echo "RetVal="."-1";
    }
    mysql_close($con);
?>
