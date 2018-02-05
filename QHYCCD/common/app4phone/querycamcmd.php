<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT DefaultCamId FROM user WHERE(UserName=$username AND PassWord=$password)");
    if($row = mysql_fetch_array($result))
    {       
        $result = mysql_query("SELECT * FROM camcmd");
        if($result)
        {
            echo 0;
            echo "<br />";
            $row = mysql_fetch_array($result);
            
            $camexp = $row['CamExp'];
            echo "CapExp:$camexp";
            echo "<br />";
            $cambin = $row['CamBin'];
            echo "CamBin:$cambin";
            echo "<br />";
            $camspeed = $row['CamSpeed'];
            echo "CamSpeed:$camspeed";
            echo "<br />";
            $camcapmode = $row['CamCapMode'];
            echo "CamCapMode:$camcapmode";
            echo "<br />";
            $rundone = $row['RunDone'];
            echo "RunDone:$rundone";
            echo "<br />";
            $cmdtype = $row['CmdType'];
            echo "CmdType:$cmdtype";
            echo "<br />";
            $focstartx = $row['FocStartX'];
            echo "FocStartX:$focstartx";
            echo "<br />";
            $focstarty = $row['FocStartY'];
            echo "FocStartY:$focstarty";
            echo "<br />";
            $cfwpos = $row['CFWPOS'];
            echo "CFWPOS:$cfwpos";
            echo "<br />";

            $result = mysql_query("SELECT * FROM status");
            if($result)
            {
                $row = mysql_fetch_array($result);
                $targettemperature = $row['TargetTemperature'];
                echo "TargetTemperature:$targettemperature";
                echo "<br />";
            }
        } 
        else
        {
            echo -11;
        }  
    }
    else
    {
        echo -2;
    }

?>
