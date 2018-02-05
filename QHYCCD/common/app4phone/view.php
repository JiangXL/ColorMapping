<?php
    $camid = isset($_GET['camid'])?$_GET['camid']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT LastImageName FROM status WHERE CamId=$camid");
    if($row = mysql_fetch_array($result))
    {
        $lastimagename = $row['LastImageName'];        
        echo "RetVal=0".","."ImageNmae=".$lastimagename;
    }
    else
    {
        echo "RetVal=-5";   
    }

?>
