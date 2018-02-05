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
        $camid = $row['DefaultCamId'];
        
        $result = mysql_query("SELECT CamConnectStatus FROM status WHERE CamId='$camid'");
        if($row = mysql_fetch_array($result))
        {
            $camconnectstatus = $row['CamConnectStatus'];
        
            if($camconnectstatus == "0")
            {
                echo "RetVal="."0".","."CamId=".$camid;
            }
        }
        else
        {
            echo "RetVal="."-4";
        }
    }
    else
    {
        echo "RetVal="."-2";
    }

?>
