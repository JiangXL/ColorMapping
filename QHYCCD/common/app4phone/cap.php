<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;
    $camexp = isset($_GET['camexp'])?$_GET['camexp']:0;
    $cambin = isset($_GET['cambin'])?$_GET['cambin']:0;
    $camspeed = isset($_GET['camspeed'])?$_GET['camspeed']:0;
    $camcapmode = isset($_GET['camcapmode'])?$_GET['camcapmode']:0;
    $rundone = isset($_GET['rundone'])?$_GET['rundone']:0;
    $cmdtype = isset($_GET['cmdtype'])?$_GET['cmdtype']:0;
    $focstartx = isset($_GET['focstartx'])?$_GET['focstartx']:0;
    $focstarty = isset($_GET['focstarty'])?$_GET['focstarty']:0;
    $gearname = isset($_GET['gearname'])?$_GET['gearname']:0;
    $cfwpos = isset($_GET['cfwpos'])?$_GET['cfwpos']:0;

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

        $result = mysql_query("SELECT CamStatus FROM status WHERE CamId='$camid'");
        if($row = mysql_fetch_array($result))
        {
            $camstatus = $row['CamStatus'];
         
            if($camstatus == 0)
            {
                $result = mysql_query("SELECT * FROM camcmd where CamId='$camid'"); 
                if($row = mysql_fetch_array($result))
                {
                    mysql_query("UPDATE camcmd set CamExp=$camexp where CamId='$camid'"); 
                    mysql_query("UPDATE camcmd set CamBin=$cambin where CamId='$camid'");
                    mysql_query("UPDATE camcmd set CamSpeed=$camspeed where CamId='$camid'");
                    mysql_query("UPDATE camcmd set CamCapMode=$camcapmode where CamId='$camid'"); 
                    mysql_query("UPDATE camcmd set RunDone=$rundone where CamId='$camid'"); 
                    mysql_query("UPDATE camcmd set CmdType=$cmdtype where CamId='$camid'");   
                    mysql_query("UPDATE camcmd set FocStartX=$focstartx where CamId='$camid'");    
                    mysql_query("UPDATE camcmd set FocStartY=$focstarty where CamId='$camid'"); 
                    mysql_query("UPDATE camcmd set CFWPOS=$cfwpos where CamId='$camid'");    
                    mysql_query("UPDATE user set DefaultGearName=$gearname where UserName=$username");        
                    echo "RetVal=0"; 
                }
                else
                {
                    $result = mysql_query("INSERT INTO camcmd (CamId,CamExp,CamBin,CamSpeed,CamCapMode,RunDone,CmdType,FocStartX,FocStartY) VALUES ('$camid','$camexp','$cambin','$camspeed','$camcapmode','$rundone','$cmdtype','$focstartx','$focstarty')");
                    echo "RetVal=0";
                }
            }
            else
            {
                echo "RetVal=-4";
            }
        }
        else
        {
           echo "RetVal=-3";
        }   
    }
    else
    {
        echo "RetVal=-2";
    }

?>
