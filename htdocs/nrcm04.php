<?php

if(isset($_GET["O2"])) //Überprüfen ob O2 übergeben wurde 
{
   $O2 = $_GET["O2"]; //Wert aus der HTTP GET anfrage wird gelesen und gespeichert 

   $servername = "localhost";
   $username = "root";
   $password = "Amor1992";  //vergebenes root Passwort 
   $dbname = "sensoren"; //Name der Datenbank 

  
   $conn = new mysqli($servername, $username, $password, $dbname); // Starten der Verbindung 
 
   if ($conn->connect_error) // Überprüfen der Verbindung 
   {
      die("Connection failed: " . $conn->connect_error);
   }

   $sql = "INSERT INTO nrcm04 (wert) VALUES ($O2)"; //gespeichert Wert wird in Tabelle nrcm04 gespeichert 

   if ($conn->query($sql) === TRUE) //Wenn Wert erfolgreich gespeichert erfolgt Ausgabe 
   {
      echo "New record created successfully";
   } else {
      echo "Error: " . $sql . " => " . $conn->error;    //Ausgabe des Fehlercodes 
   }

   $conn->close();
} else {
   echo "O2 is not set"; //Wenn in der GET Anfrage kein O2 übergeben wurde
}
?>
