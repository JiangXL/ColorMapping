<?php

    $cmd = isset($_GET['cmd'])?$_GET['cmd']:'';
    $username = isset($_GET['username'])?$_GET['username']:'';    
    $gearname = isset($_GET['gearname'])?$_GET['gearname']:'';
    $camname = isset($_GET['camname'])?$_GET['camname']:'';
    $ownername = isset($_GET['ownername'])?$_GET['ownername']:'';
    $emailaddress = isset($_GET['emailaddress'])?$_GET['emailaddress']:'';
    $observatorylocationname = isset($_GET['observatorylocationname'])?$_GET['observatorylocationname']:'';
    $observatoryname = isset($_GET['observatoryname'])?$_GET['observatoryname']:'';
    $observatoryinternationalcode = isset($_GET['observatoryinternationalcode'])?$_GET['observatoryinternationalcode']:'';
    $longitude = isset($_GET['longitude'])?$_GET['longitude']:'';
    $latitude = isset($_GET['latitude'])?$_GET['latitude']:'';
    $timezone = isset($_GET['timezone'])?$_GET['timezone']:'';
    $telescopemodel = isset($_GET['telescopemodel'])?$_GET['telescopemodel']:''; 
    $diameter = isset($_GET['diameter'])?$_GET['diameter']:'';
    $aperture = isset($_GET['aperture'])?$_GET['aperture']:'';
    $telescopefocallength = isset($_GET['telescopefocallength'])?$_GET['telescopefocallength']:'';
    $flattermodel = isset($_GET['flattermodel'])?$_GET['flattermodel']:'';
    $flatterratio = isset($_GET['flatterratio'])?$_GET['flatterratio']:'';
    $favoritetemperaturesummer = isset($_GET['favoritetemperaturesummer'])?$_GET['favoritetemperaturesummer']:'';
    $favoritetemperaturewinter = isset($_GET['favoritetemperaturewinter'])?$_GET['favoritetemperaturewinter']:'';
    $favoriteexposuretime = isset($_GET['favoriteexposuretime'])?$_GET['favoriteexposuretime']:'';
    $favoriteexposuretimenum = isset($_GET['favoriteexposuretimenum'])?$_GET['favoriteexposuretimenum']:'';
    $offsetbin11hs = isset($_GET['offsetbin11hs'])?$_GET['offsetbin11hs']:'';
    $offsetbin22hs = isset($_GET['offsetbin22hs'])?$_GET['offsetbin22hs']:'';
    $offsetbin33hs = isset($_GET['offsetbin33hs'])?$_GET['offsetbin33hs']:'';
    $offsetbin44hs = isset($_GET['offsetbin44hs'])?$_GET['offsetbin44hs']:'';
    $gainbin11hs = isset($_GET['gainbin11hs'])?$_GET['gainbin11hs']:'';
    $gainbin22hs = isset($_GET['gainbin22hs'])?$_GET['gainbin22hs']:'';
    $gainbin33hs = isset($_GET['gainbin33hs'])?$_GET['gainbin33hs']:'';
    $gainbin44hs = isset($_GET['gainbin44hs'])?$_GET['gainbin44hs']:'';
    $offsetbin11ls = isset($_GET['offsetbin11ls'])?$_GET['offsetbin11ls']:'';
    $offsetbin22ls = isset($_GET['offsetbin22ls'])?$_GET['offsetbin22ls']:'';
    $offsetbin33ls = isset($_GET['offsetbin33ls'])?$_GET['offsetbin33ls']:'';
    $offsetbin44ls = isset($_GET['offsetbin44ls'])?$_GET['offsetbin44ls']:'';
    $gainbin11ls = isset($_GET['gainbin11ls'])?$_GET['gainbin11ls']:'';
    $gainbin22ls = isset($_GET['gainbin22ls'])?$_GET['gainbin22ls']:'';
    $gainbin33ls = isset($_GET['gainbin33ls'])?$_GET['gainbin33ls']:'';
    $gainbin44ls = isset($_GET['gainbin44ls'])?$_GET['gainbin44ls']:'';

    $con = mysql_connect("localhost","root","root");
    if (!$con)
    {
        die('Could not connect: ' . mysql_error());
    }

    mysql_select_db("ic8300", $con);
   
    if($cmd == 0)
    {
        /*insert*/
        $result = mysql_query("INSERT INTO gear (GearName,UserName) VALUES ($gearname,$username)");

        /*update*/
        if($gearname)
            $result = mysql_query("UPDATE gear set GearName=$gearname WHERE(GearName=$gearname AND UserName=$username)");
        if($camname)
            $result = mysql_query("UPDATE gear set CamName=$camname WHERE(GearName=$gearname AND UserName=$username)");
        if($ownername)
            $result = mysql_query("UPDATE gear set OwnerName=$ownername WHERE(GearName=$gearname AND UserName=$username)");   
        if($emailaddress)
            $result = mysql_query("UPDATE gear set EmailAddress=$emailaddress WHERE(GearName=$gearname AND UserName=$username)");     
        if($observatorylocationname)
            $result = mysql_query("UPDATE gear set ObservatoryLocationName=$observatorylocationname WHERE(GearName=$gearname AND UserName=$username)"); 
        if($observatoryname)
           $result = mysql_query("UPDATE gear set ObservatoryName=$observatoryname WHERE(GearName=$gearname AND UserName=$username)"); 
        if($observatoryinternationalcode)
            $result = mysql_query("UPDATE gear set ObservatoryInternationalCode=$observatoryinternationalcode WHERE(GearName=$gearname AND UserName=$username)");   
        if($longitude)
            $result = mysql_query("UPDATE gear set Longitude=$longitude WHERE(GearName=$gearname AND UserName=$username)");
        if($latitude)
            $result = mysql_query("UPDATE gear set Latitude=$latitude WHERE(GearName=$gearname AND UserName=$username)"); 
        if($timezone)
            $result = mysql_query("UPDATE gear set TimeZone=$timezone WHERE(GearName=$gearname AND UserName=$username)"); 
        if($telescopemodel)
            $result = mysql_query("UPDATE gear set TelescopeModel=$telescopemodel WHERE(GearName=$gearname AND UserName=$username)");   
        if($diameter)
            $result = mysql_query("UPDATE gear set Diameter=$diameter WHERE(GearName=$gearname AND UserName=$username)"); 
        if($aperture)
            $result = mysql_query("UPDATE gear set Aperture=$aperture WHERE(GearName=$gearname AND UserName=$username)"); 
        if($telescopefocallength)
            $result = mysql_query("UPDATE gear set TelescopeFocalLength=$telescopefocallength WHERE(GearName=$gearname AND UserName=$username)"); 
        if($flattermodel)
            $result = mysql_query("UPDATE gear set FlatterModel=$flattermodel WHERE(GearName=$gearname AND UserName=$username)");   
        if($flatterratio)
            $result = mysql_query("UPDATE gear set FlatterRatio=$flatterratio WHERE(GearName=$gearname AND UserName=$username)"); 
        if($favoritetemperaturesummer)
            $result = mysql_query("UPDATE gear set FavoriteTemperatureSummer=$favoritetemperaturesummer WHERE(GearName=$gearname AND UserName=$username)"); 
        if($favoritetemperaturewinter)
            $result = mysql_query("UPDATE gear set FavoriteTemperatureWinter=$favoritetemperaturewinter WHERE(GearName=$gearname AND UserName=$username)"); 
        if($favoriteexposuretime)
            $result = mysql_query("UPDATE gear set FavoriteExposureTime=$favoriteexposuretime WHERE(GearName=$gearname AND UserName=$username)");   
        if($favoriteexposuretimenum)
            $result = mysql_query("UPDATE gear set FavoriteExposureTimeNum=$favoriteexposuretimenum WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin11hs)
            $result = mysql_query("UPDATE gear set OffsetBIN11HS=$offsetbin11hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin22hs)
            $result = mysql_query("UPDATE gear set OffsetBIN22HS=$offsetbin22hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin33hs)
            $result = mysql_query("UPDATE gear set OffsetBIN33HS=$offsetbin33hs WHERE(GearName=$gearname AND UserName=$username)");   
        if($offsetbin44hs)
            $result = mysql_query("UPDATE gear set OffsetBIN44HS=$offsetbin44hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin11hs)
            $result = mysql_query("UPDATE gear set GainBIN11HS=$gainbin11hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin22hs)
            $result = mysql_query("UPDATE gear set GainBIN22HS=$gainbin22hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin33hs)
            $result = mysql_query("UPDATE gear set GainBIN33HS=$gainbin33hs WHERE(GearName=$gearname AND UserName=$username)");   
        if($gainbin44hs)
            $result = mysql_query("UPDATE gear set GainBIN44HS=$gainbin44hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin11ls)
            $result = mysql_query("UPDATE gear set OffsetBIN11LS=$offsetbin11ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin22ls)
            $result = mysql_query("UPDATE gear set OffsetBIN22LS=$offsetbin22ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin33ls)
            $result = mysql_query("UPDATE gear set OffsetBIN33LS=$offsetbin33ls WHERE(GearName=$gearname AND UserName=$username)");   
        if($offsetbin44ls)
            $result = mysql_query("UPDATE gear set OffsetBIN44LS=$offsetbin44ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin11ls)
            $result = mysql_query("UPDATE gear set GainBIN11LS=$gainbin11ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin22ls)
            $result = mysql_query("UPDATE gear set GainBIN22LS=$gainbin22ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin33ls)
            $result = mysql_query("UPDATE gear set GainBIN33LS=$gainbin33ls WHERE(GearName=$gearname AND UserName=$username)");   
        if($gainbin44ls)
            $result = mysql_query("UPDATE gear set GainBIN44LS=$gainbin44ls WHERE(GearName=$gearname AND UserName=$username)"); 
        $result = 1 - $result;
        echo "RetVal=".$result;
    }
    else if($cmd == 1)
    {
        /*update*/
        if($gearname)
            $result = mysql_query("UPDATE gear set GearName=$gearname WHERE(GearName=$gearname AND UserName=$username)");
        if($camname)
            $result = mysql_query("UPDATE gear set CamName=$camname WHERE(GearName=$gearname AND UserName=$username)");
        if($ownername)
            $result = mysql_query("UPDATE gear set OwnerName=$ownername WHERE(GearName=$gearname AND UserName=$username)");   
        if($emailaddress)
            $result = mysql_query("UPDATE gear set EmailAddress=$emailaddress WHERE(GearName=$gearname AND UserName=$username)");     
        if($observatorylocationname)
            $result = mysql_query("UPDATE gear set ObservatoryLocationName=$observatorylocationname WHERE(GearName=$gearname AND UserName=$username)"); 
        if($observatoryname)
           $result = mysql_query("UPDATE gear set ObservatoryName=$observatoryname WHERE(GearName=$gearname AND UserName=$username)"); 
        if($observatoryinternationalcode)
            $result = mysql_query("UPDATE gear set ObservatoryInternationalCode=$observatoryinternationalcode WHERE(GearName=$gearname AND UserName=$username)");   
        if($longitude)
            $result = mysql_query("UPDATE gear set Longitude=$longitude WHERE(GearName=$gearname AND UserName=$username)");
        if($latitude)
            $result = mysql_query("UPDATE gear set Latitude=$latitude WHERE(GearName=$gearname AND UserName=$username)"); 
        if($timezone)
            $result = mysql_query("UPDATE gear set TimeZone=$timezone WHERE(GearName=$gearname AND UserName=$username)"); 
        if($telescopemodel)
            $result = mysql_query("UPDATE gear set TelescopeModel=$telescopemodel WHERE(GearName=$gearname AND UserName=$username)");   
        if($diameter)
            $result = mysql_query("UPDATE gear set Diameter=$diameter WHERE(GearName=$gearname AND UserName=$username)"); 
        if($aperture)
            $result = mysql_query("UPDATE gear set Aperture=$aperture WHERE(GearName=$gearname AND UserName=$username)"); 
        if($telescopefocallength)
            $result = mysql_query("UPDATE gear set TelescopeFocalLength=$telescopefocallength WHERE(GearName=$gearname AND UserName=$username)"); 
        if($flattermodel)
            $result = mysql_query("UPDATE gear set FlatterModel=$flattermodel WHERE(GearName=$gearname AND UserName=$username)");   
        if($flatterratio)
            $result = mysql_query("UPDATE gear set FlatterRatio=$flatterratio WHERE(GearName=$gearname AND UserName=$username)"); 
        if($favoritetemperaturesummer)
            $result = mysql_query("UPDATE gear set FavoriteTemperatureSummer=$favoritetemperaturesummer WHERE(GearName=$gearname AND UserName=$username)"); 
        if($favoritetemperaturewinter)
            $result = mysql_query("UPDATE gear set FavoriteTemperatureWinter=$favoritetemperaturewinter WHERE(GearName=$gearname AND UserName=$username)"); 
        if($favoriteexposuretime)
            $result = mysql_query("UPDATE gear set FavoriteExposureTime=$favoriteexposuretime WHERE(GearName=$gearname AND UserName=$username)");   
        if($favoriteexposuretimenum)
            $result = mysql_query("UPDATE gear set FavoriteExposureTimeNum=$favoriteexposuretimenum WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin11hs)
            $result = mysql_query("UPDATE gear set OffsetBIN11HS=$offsetbin11hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin22hs)
            $result = mysql_query("UPDATE gear set OffsetBIN22HS=$offsetbin22hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin33hs)
            $result = mysql_query("UPDATE gear set OffsetBIN33HS=$offsetbin33hs WHERE(GearName=$gearname AND UserName=$username)");   
        if($offsetbin44hs)
            $result = mysql_query("UPDATE gear set OffsetBIN44HS=$offsetbin44hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin11hs)
            $result = mysql_query("UPDATE gear set GainBIN11HS=$gainbin11hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin22hs)
            $result = mysql_query("UPDATE gear set GainBIN22HS=$gainbin22hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin33hs)
            $result = mysql_query("UPDATE gear set GainBIN33HS=$gainbin33hs WHERE(GearName=$gearname AND UserName=$username)");   
        if($gainbin44hs)
            $result = mysql_query("UPDATE gear set GainBIN44HS=$gainbin44hs WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin11ls)
            $result = mysql_query("UPDATE gear set OffsetBIN11LS=$offsetbin11ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin22ls)
            $result = mysql_query("UPDATE gear set OffsetBIN22LS=$offsetbin22ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($offsetbin33ls)
            $result = mysql_query("UPDATE gear set OffsetBIN33LS=$offsetbin33ls WHERE(GearName=$gearname AND UserName=$username)");   
        if($offsetbin44ls)
            $result = mysql_query("UPDATE gear set OffsetBIN44LS=$offsetbin44ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin11ls)
            $result = mysql_query("UPDATE gear set GainBIN11LS=$gainbin11ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin22ls)
            $result = mysql_query("UPDATE gear set GainBIN22LS=$gainbin22ls WHERE(GearName=$gearname AND UserName=$username)"); 
        if($gainbin33ls)
            $result = mysql_query("UPDATE gear set GainBIN33LS=$gainbin33ls WHERE(GearName=$gearname AND UserName=$username)");   
        if($gainbin44ls)
            $result = mysql_query("UPDATE gear set GainBIN44LS=$gainbin44ls WHERE(GearName=$gearname AND UserName=$username)"); 
        $result = 1 - $result;
        echo "RetVal=".$result;
    }
    else if($cmd == 2)
    {
        /*del*/
        $result = mysql_query("DELETE FROM gear WHERE GearName=$gearname");
        $result = 1 - $result;
        echo "RetVal=".$result;
    }
    else if($cmd == 3)
    {   
        $result = mysql_query("SELECT * FROM gear WHERE(GearName=$gearname AND UserName=$username)");
        if($row = mysql_fetch_array($result))
        {
            $gearname = $row['GearName'];
            $camname = $row['CamName'];
            $ownername = $row['OwnerName'];
            $emailaddress = $row['EmailAddress'];
            $observatorylocationname = $row['ObservatoryLocationName'];  
            $observatoryname = $row['ObservatoryName'];
            $observatoryinternationalcode = $row['ObservatoryInternationalCode'];
            $longitude = $row['Longitude'];
            $latitude = $row['Latitude'];
            $timezone = $row['TimeZone'];
            $telescopemodel = $row['TelescopeModel'];
            $diameter = $row['Diameter'];
            $aperture = $row['Aperture'];
            $telescopefocallength = $row['TelescopeFocalLength'];
            $flattermodel = $row['FlatterModel'];
            $flatterratio = $row['FlatterRatio'];
            $favoritetemperaturesummer = $row['FavoriteTemperatureSummer'];
            $favoritetemperaturewinter = $row['FavoriteTemperatureWinter'];
            $favoriteexposuretime = $row['FavoriteExposureTime'];
            $favoriteexposuretimenum = $row['FavoriteExposureTimeNum'];
            $offsetbin11hs = $row['OffsetBIN11HS'];
            $offsetbin22hs  = $row['OffsetBIN22HS'];
            $offsetbin33hs  = $row['OffsetBIN33HS'];
            $offsetbin44hs  = $row['OffsetBIN44HS'];
            $gainbin11hs = $row['GainBIN11HS'];
            $gainbin22hs = $row['GainBIN22HS'];
            $gainbin33hs = $row['GainBIN33HS'];
            $gainbin44hs = $row['GainBIN44HS'];
            $offsetbin11ls = $row['OffsetBIN11LS'];
            $offsetbin22ls = $row['OffsetBIN22LS'];
            $offsetbin33ls = $row['OffsetBIN33LS'];
            $offsetbin44ls = $row['OffsetBIN44LS'];
            $gainbin11ls = $row['GainBIN11LS'];
            $gainbin22ls = $row['GainBIN22LS'];
            $gainbin33ls = $row['GainBIN33LS'];
            $gainbin44ls = $row['GainBIN44LS'];  

            echo "GearName=".$gearname.","."CamName=".$camname.","."OwnerName=".$ownername.","."EmailAddress=".$emailaddress.",".
                 "ObservatoryLocationName=".$observatorylocationname.","."ObservatoryName=".$observatoryname.","."ObservatoryInternationalCode=".$observatoryinternationalcode.","."Longitude=".$longitude.",".
                 "Latitude=".$latitude.","."TimeZone=".$timezone.","."TelescopeModel=".$telescopemodel.","."Diameter=".$diameter.",".
                 "Aperture=".$aperture.","."TelescopeFocalLength=".$telescopefocallength.","."FlatterModel=".$flattermodel.","."FlatterRatio=".$flatterratio.",".
                 "FavoriteTemperatureSummer=".$favoritetemperaturesummer.","."FavoriteTemperatureWinter=".$favoritetemperaturewinter.","."FavoriteExposureTime=".$favoriteexposuretime.","."FavoriteExposureTimeNum=".$favoriteexposuretimenum.",".
                 "OffsetBIN11HS=".$offsetbin11hs.","."OffsetBIN22HS=".$offsetbin22hs.","."OffsetBIN33HS=".$offsetbin33hs.","."OffsetBIN44HS=".$offsetbin44hs.",".
                 "GainBIN11HS=".$gainbin11hs.","."GainBIN22HS=".$gainbin22hs.","."GainBIN33HS=".$gainbin33hs.","."GainBIN44HS=".$gainbin44hs.",".
                 "OffsetBIN11LS=".$offsetbin11ls.","."OffsetBIN22LS=".$offsetbin22ls.","."OffsetBIN33LS=".$offsetbin33ls.","."OffsetBIN44LS=".$offsetbin44ls.",".
                 "GainBIN11LS=".$gainbin11ls.","."GainBIN22LS=".$gainbin22ls.","."GainBIN33LS=".$gainbin33ls.","."GainBIN44LS=".$gainbin44ls;
        }
    }
    else if($cmd == 4)
    {
        $result = mysql_query("SELECT GearName FROM gear WHERE UserName=$username");
        while($row = mysql_fetch_array($result))
        {
            $gearname = $row['GearName'];
            echo $gearname;
            echo "<br />";
        }  
    }
?>
