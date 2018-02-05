<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;
    $targettemperature = isset($_GET['targettemperature'])?$_GET['targettemperature']:0;
    $camstatus = isset($_GET['camstatus'])?$_GET['camstatus']:-1;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT DefaultCamId FROM user WHERE(UserName=$username AND PassWord=$password)");
    if($row = mysql_fetch_array($result))
    {
        $camid = $row['DefaultCamId'];
        
        $result = mysql_query("UPDATE status SET TargetTemperature=$targettemperature WHERE CamId='$camid'");

        if($camstatus != -1)
             $result = mysql_query("UPDATE status SET CamStatus=$camstatus WHERE CamId='$camid'");


        echo "RetVal=0";
    }
    else
    {
        echo -2;
    }

?>
