<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;
    $cmd = isset($_GET['cmd'])?$_GET['cmd']:0;
    $subindex = isset($_GET['subindex'])?$_GET['subindex']:0;
    $plannername = isset($_GET['plannername'])?$_GET['plannername']:0;
    $use = isset($_GET['use'])?$_GET['use']:0;
    $bin = isset($_GET['bin'])?$_GET['bin']:0;
    $exptime = isset($_GET['exptime'])?$_GET['exptime']:0;
    $repeat = isset($_GET['repeat'])?$_GET['repeat']:0;
    $cfw = isset($_GET['cfw'])?$_GET['cfw']:0;
    $delay = isset($_GET['delay'])?$_GET['delay']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);
    
    if($cmd == 0)
    {
        /*insert*/
        $result = mysql_query("INSERT INTO planner (SubIndex,PlannerName,UserName,IsUse,BIN,ExpTime,RepeatNum,CFW,Delay) VALUES($subindex,$plannername,$username,$use,$bin,$exptime,$repeat,$cfw,$delay)");
        $result = 1 - $result;
        echo "RetVal=".$result;
    }
    else if($cmd == 1)
    {
        /*update*/
        mysql_query("UPDATE planner set IsUse=$use where(PlannerName=$plannername AND SubIndex=$subindex)"); 
        mysql_query("UPDATE planner set BIN=$bin where(PlannerName=$plannername AND SubIndex=$subindex)");
        mysql_query("UPDATE planner set ExpTime=$exptime where(PlannerName=$plannername AND SubIndex=$subindex)");
        mysql_query("UPDATE planner set RepeatNum=$repeat where(PlannerName=$plannername AND SubIndex=$subindex)"); 
        mysql_query("UPDATE planner set CFW=$cfw where(PlannerName=$plannername AND SubIndex=$subindex)"); 
        mysql_query("UPDATE planner set Delay=$delay where(PlannerName=$plannername AND SubIndex=$subindex)"); 
        echo "RetVal=0";
    }
    else if($cmd == 2)
    {
        /*del*/
        mysql_query("DELETE FROM planner WHERE (SubIndex=$subindex AND PlannerName=$plannername)");
        echo "RetVal=0";
    }
    else
    {
        echo -6;
        return;
    }
?>
