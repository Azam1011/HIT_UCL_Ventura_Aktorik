<?php

if(isset($_GET["pressure"])) //Überprüfen ob pressure übergeben wurde 
 {
   $pressure = $_GET["pressure"]; //Wert aus der HTTP GET anfrage wird gelesen und gespeichert 

   $servername = "localhost";
   $username = "root";
   $password = "Amor1992";  //vergebenes root Passwort 
   $dbname = "sensoren"; //Name der Datenbank 


   $conn = new mysqli($servername, $username, $password, $dbname); // Starten der Verbindung 

   if ($conn->connect_error) // Überprüfen der Verbindung 
   {
      die("Connection failed: " . $conn->connect_error);
   }

   $sql = "INSERT INTO ams6915 (wert) VALUES ($pressure)"; //gespeichert Wert wird in Tabelle ams6915 gespeichert 

   if ($conn->query($sql) === TRUE)  //Wenn Wert erfolgreich gespeichert erfolgt Ausgabe 
   {
      echo "New record created successfully";
   } else {
      echo "Error: " . $sql . " => " . $conn->error;    //Ausgabe des Fehlercodes 
   }

   $conn->close();
} else {
   echo "pressure is not set";  //Wenn in der GET Anfrage kein pressure übergeben wurde
}
?>
