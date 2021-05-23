<?php

if(isset($_GET["flow"])) //Überprüfen ob flow übergeben wurde 
{
   $flow = $_GET["flow"]; //Wert aus der HTTP GET anfrage wird gelesen und gespeichert 

   $servername = "localhost";   
   $username = "root";
   $password = "Amor1992";          //vergebenes root Passwort 
   $dbname = "sensoren";    //Name der Datenbank 

   
   $conn = new mysqli($servername, $username, $password, $dbname); // Starten der Verbindung 
   
   if ($conn->connect_error)  // Überprüfen der Verbindung 
   {
      die("Connection failed: " . $conn->connect_error); 
   }

   $sql = "INSERT INTO sfm3019 (wert) VALUES ($flow)";  //gespeichert Wert wird in Tabelle sfm3019 gespeichert 

   if ($conn->query($sql) === TRUE)  //Wenn Wert erfolgreich gespeichert erfolgt Ausgabe 
   {
      echo "New record created successfully";      
   } else {
      echo "Error: " . $sql . " => " . $conn->error; //Ausgabe des Fehlercodes 
   }

   $conn->close();
} else {
   echo "flow is not set";  //Wenn in der GET Anfrage kein flow übergeben wurde
}
?>
