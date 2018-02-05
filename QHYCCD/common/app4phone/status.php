<?php
    $camid = isset($_GET['camid'])?$_GET['camid']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT CamStatus,Temperature,ExpSchedule,ReadOutSchedule,CamConnectStatus,TotalPlanSchedule FROM status WHERE CamId=$camid");
    if($row = mysql_fetch_array($result))
    {
        $camstatus = $row['CamStatus'];
        $temperature = $row['Temperature'];
        $expschedule = $row['ExpSchedule'];
        $readoutschedule = $row['ReadOutSchedule'];
        $camconnectstatus = $row['CamConnectStatus'];
        $totalplanschedule = $row['TotalPlanSchedule'];        

        echo "RetVal=0".","."CamStatus=".$camstatus.","."Temperature=".$temperature.","."ExpSchedule=".$expschedule.","."ReadOutSchedule=".$readoutschedule.","."CamConnectStatus=".$camconnectstatus.","."TotalPlanSchedule=".$totalplanschedule;

    }
    else
    {
        $result = 1 - $result;
        echo "RetVal=".$result;
    }

?>
