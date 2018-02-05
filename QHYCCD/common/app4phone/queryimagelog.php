<?php
    $username = isset($_GET['username'])?$_GET['username']:0;
    $password = isset($_GET['password'])?$_GET['password']:0;
    $cmd = isset($_GET['cmd'])?$_GET['cmd']:0;
    $dirname = isset($_GET['dirname'])?$_GET['dirname']:0;
    $imgname = isset($_GET['imgname'])?$_GET['imgname']:0;

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);

    $result = mysql_query("SELECT DefaultCamId FROM user WHERE(UserName=$username AND PassWord=$password)");
    if($row = mysql_fetch_array($result))
    {
        if($cmd == 0)
        {        
            $result = mysql_query("SELECT ImageName FROM imagelog WHERE 1");
            if($result)
            {
                echo 0;
                echo "<br />";
                while($row = mysql_fetch_array($result))
                {
                    $ImageName = $row['ImageName'];
                    echo $ImageName;
                    echo "<br />";
                }
            }
            else
            {
                echo -7;
            } 
        }
        else if($cmd == 1)
        {
            $result = mysql_query("DELETE FROM imagelog WHERE ImageName=$imgname");
            if($result)
            {
                echo 0;
            }
            else
            {
                echo -8;
            } 

            unlink("$imgname.fit");
        }
        else if($cmd == 2)
        {
            $arr = scandir("/media/orangepi");
            $arrlength=count($arr);

            for($x=2;$x<$arrlength;$x++) 
            {
                echo $arr[$x].","."leftsize:".disk_free_space("/media/orangepi/".$arr[$x]) / (1024 * 1024)."MB";
                echo "<br>";
            }
        }
        else if($cmd == 3)
        {
            $leftsize = disk_free_space("/media/orangepi/$dirname");

            $fsize = filesize("$imgname.fit");

            if($fsize > $leftsize)
            {
                echo -9;
                return;
            }
            else
            {
                if(!copy("$imgname.fit","/media/orangepi/$dirname/$imgname.fit"))
                {
                    echo -10;
                }
                else
                {
                    echo 0;
                }
            }
        }
    }
    else
    {
        echo -2;
    }

?>
