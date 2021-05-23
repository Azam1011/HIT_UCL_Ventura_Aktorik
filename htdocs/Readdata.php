<?php

$servername = "localhost";
$username = "root";
$password = "Amor1992";  //vergebenes root Passwort 
$dbname = "sensoren"; //Name der Datenbank 
$conn = new mysqli($servername, $username, $password, $dbname); // Starten der Verbindung

if ($conn->connect_error) // Überprüfen der Verbindung 
{ 
   die("Connection failed: " . $conn->connect_error); 
}

if (isset($_GET["Sensor"]))
{
    $Sensor = $_GET["Sensor"];

    // Funktion zum Datalesen von MySQL Database
    function readdata($conn,$Sensor)
    {


        $sql = "SELECT wert FROM $Sensor ORDER BY id DESC LIMIT 1;"; //Wert wird aus Tabelle abgelesen 
        
        $result = mysqli_query($conn,$sql);

        if ($result)  //Wenn Wert erfolgreich gespeichert erfolgt Ausgabe 
        {             
            $Data = $result->fetch_assoc();
            echo  $Data["wert"];
        } 
        else 
        {
            echo "Error: " . $sql . " => " . $conn->error; //Ausgabe des Fehlercodes 
        }
    }

    readdata($conn,$Sensor);
}

else
{
    echo "No sensor name found";
}

?>