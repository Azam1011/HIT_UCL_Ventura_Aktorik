<?php
$servername = "localhost";
$username = "root";
$password = "Amor1992";  //vergebenes root Passwort 
$dbname = "user_input"; //Name der Datenbank 
$conn = new mysqli($servername, $username, $password, $dbname); // Starten der Verbindung

if ($conn->connect_error) // Überprüfen der Verbindung 
{ 
   die("Connection failed: " . $conn->connect_error); 
}

if (isset($_GET["flow_in_input"]))
{
    $User_Input = $_GET["flow_in_input"];

    // Funktion zum Datalesen von MySQL Database
    function writedata($conn,$User_Input)
    {

        $sql = "UPDATE user_input.flow_in_input SET wert = $User_Input WHERE id ORDER BY id DESC LIMIT 1;"; //Wert wird aus Tabelle abgelesen 

        if ($conn->query($sql) === TRUE)  //Wenn Wert erfolgreich gespeichert erfolgt Ausgabe 
        {             
            echo  "Success";
        } 
        else 
        {
            echo "Error: " . $sql . " => " . $conn->error; //Ausgabe des Fehlercodes 
        }
    }

    writedata($conn,$User_Input);
}

elseif (isset($_GET["User_Input"]))
{
    $User_Input = $_GET["User_Input"];

    // Funktion zum Datalesen von MySQL Database
    function readdata($conn,$User_Input)
    {

        $sql = "SELECT wert FROM $User_Input ORDER BY id DESC LIMIT 1;"; //Wert wird aus Tabelle abgelesen 
        
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

    readdata($conn,$User_Input);
}
elseif (isset($_GET["on_off"]))
{
    $User_Input = $_GET["on_off"];

    // Funktion zum Datalesen von MySQL Database
    function writedata($conn,$User_Input)
    {


        $sql = "UPDATE user_input.on_off SET wert = $User_Input WHERE id ORDER BY id DESC LIMIT 1;"; //Wert wird aus Tabelle abgelesen 

        if ($conn->query($sql) === TRUE)  //Wenn Wert erfolgreich gespeichert erfolgt Ausgabe 
        {             
            echo  "Success";
        } 
        else 
        {
            echo "Error: " . $sql . " => " . $conn->error; //Ausgabe des Fehlercodes 
        }
    }

    writedata($conn,$User_Input);
}
elseif (isset($_GET["o2_input"]))
{
    $User_Input = $_GET["o2_input"];

    // Funktion zum Datalesen von MySQL Database
    function writedata($conn,$User_Input)
    {


        $sql = "UPDATE user_input.o2_input SET wert = $User_Input WHERE id ORDER BY id DESC LIMIT 1;"; //Wert wird aus Tabelle abgelesen 

        if ($conn->query($sql) === TRUE)  //Wenn Wert erfolgreich gespeichert erfolgt Ausgabe 
        {             
            echo  "Success";
        } 
        else 
        {
            echo "Error: " . $sql . " => " . $conn->error; //Ausgabe des Fehlercodes 
        }
    }

    writedata($conn,$User_Input);
}
else
{
    echo "Input data not found";
}
?>
