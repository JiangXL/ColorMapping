<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;
    $camid = isset($_GET['camid'])?$_GET['camid']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT DefaultCamId FROM user WHERE(UserName=$username AND PassWord=$password)");
    if($row = mysql_fetch_array($result))
    {
        $result = mysql_query("UPDATE user set DefaultCamId=$camid WHERE(UserName=$username AND PassWord=$password)");

        echo "RetVal=0";  
    }
    else
    {
        echo "RetVal=-2";
    }

?>
