<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;
    $plannername = isset($_GET['plannername'])?$_GET['plannername']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT DefaultCamId FROM user WHERE(UserName=$username AND PassWord=$password)");
    if($row = mysql_fetch_array($result))
    {
        if($plannername)
        {
            $result = mysql_query("SELECT SubIndex,IsUse,BIN,ExpTime,RepeatNum,CFW,Delay FROM planner WHERE PlannerName=$plannername");
            //echo "SubIndex,IsUse,BIN,ExpTime,RepeatNum,CFW,Delay".'<br />';

            while($row = mysql_fetch_array($result))
            {
                $subindex = $row['SubIndex'];
                $isuse = $row['IsUse'];
                $bin = $row['BIN'];
                $exptime = $row['ExpTime'];
                $repeatnum = $row['RepeatNum'];
                $cfw = $row['CFW'];
                $delay = $row['Delay'];
                echo $subindex.",".$isuse.",".$bin.",".$exptime.",".$repeatnum.",".$cfw.",".$delay;
                echo '<br />';
            }
        }
        else
        {
            $result = mysql_query("SELECT PlannerName FROM planner WHERE UserName=$username GROUP by PlannerName");
            //echo "PlannerName".'<br />';

            while($row = mysql_fetch_array($result))
            {
                $plannername = $row['PlannerName'];
                if($lastplannername != $plannername)
                {          
                    $lastplannername = $plannername;     
                    echo $plannername;
                    echo '<br />';
                }
            }

        }
    }
    else
    {
        echo "RetVal=-2";
    }
?>
