
var readSensor_url = 'Readdata.php'; //A local page
var input_url = 'Inputdata.php'; //A local page


// Urls to fetch Sensor data from database
O2_url = readSensor_url+'?Sensor=nrcm04';
Pressure_url = readSensor_url+'?Sensor=ams6915';
Flow_OUT_url = readSensor_url+'?Sensor=sfm3200aw';
Flow_IN_url = readSensor_url+'?Sensor=sfm3019';

// Urls to fetch Input data from database
flow_in_input_read_url = input_url + '?User_Input=flow_in_input';
on_off_read_url = input_url + '?User_Input=on_off';
o2_input_read_url = input_url + '?User_Input=o2_input';

// Urls to write Input data in database
flow_in_input_update_url = input_url + '?flow_in_input=';
on_off_update_url = input_url + '?on_off=';
o2_input_update_url = input_url + '?o2_input=';


// Write sensor data value in Website
function writeSensorValue(value,id)
{
    document.getElementById(id).innerHTML = value;
}

// Send Get request to fetch sensor data and call callback function to 
// write value in user interface (Website)
function loadSensorData(url, callback,mid) {
  var xhr = new XMLHttpRequest();

  xhr.onreadystatechange = function() {
    if (xhr.readyState === xhr.DONE && xhr.status === 200) {
        callback(xhr.response,mid);
    }
  }

  xhr.open('GET', url, true);
  xhr.send('');
}

// Write user input data value in Website
function writeUserInputValue(value,mid)
{
  if (document.getElementById(mid).value != value)
  {
    document.getElementById(mid).value = value;
    if (mid === 'Flow_in_Input')
    {
      Flow_in_changed();
    }
    else
    {
      O2changed();
    }
  }
}

// Send Get request to fetch user input data from database and call callback function to 
// write value in user interface (Website)
function loadUserInputData(url, callback,mid) {
  var xhr = new XMLHttpRequest();

  xhr.onreadystatechange = function() {
    if (xhr.readyState === xhr.DONE && xhr.status === 200) {
        callback(xhr.response,mid);
    }
  }

  xhr.open('GET', url, true);
  xhr.send('');
}

// Write user input data value in Website
function writeOnOffInputValue(val, mid)
{
  if (document.getElementById(mid).checked != val)
  {
    document.getElementById(mid).checked = Number(val);
    On_Off_changed();
  }

}


// Send Get request to update user input data in database and call callback function to 
// write value in user interface (Website)
function UploadUserInputData(url,val) {
  var xhr = new XMLHttpRequest();

  url = url + val;
  xhr.onreadystatechange = function() {
    if (xhr.readyState === 4) {
        //callback(xhr.response);
    }
  }

  xhr.open('GET', url, true);
  xhr.send('');
}

// read all user input values from database and update the user interface
function readAllInputData()
{
  loadUserInputData(flow_in_input_read_url, writeUserInputValue, 'Flow_in_Input');
  loadUserInputData(o2_input_read_url, writeUserInputValue, 'O2_Input');
  loadUserInputData(on_off_read_url, writeOnOffInputValue, 'On_Off_Input');   
}

// read all Sensor values from database and update the user interface
function readAllSensorData()
{
    loadSensorData(Flow_IN_url, writeSensorValue, 'Flow_IN');   
    loadSensorData(Flow_OUT_url, writeSensorValue, 'Flow_OUT');
    loadSensorData(Pressure_url, writeSensorValue, 'Pressure');
    loadSensorData(O2_url, writeSensorValue, 'O2');
}

// When first open the Website
window.onload = function() 
{
    readAllSensorData();
    readAllInputData();
    O2changed();
    Flow_in_changed();
    On_Off_changed();
}

// write Input value in number
function O2changed()
{
  var val = document.getElementById('O2_Input').value;
  document.getElementById('O2_Value').innerHTML = val;
  //document.getElementById('O2_Input').value = val;
  UploadUserInputData(o2_input_update_url,val);
}

// write Input value in number
function Flow_in_changed()
{
  var val = document.getElementById('Flow_in_Input').value;
  document.getElementById('Flow_in_Value').innerHTML = val;
  //document.getElementById('O2_Input').value = val;
  UploadUserInputData(flow_in_input_update_url,val);
}

// Write changed Input value : On or OFF
function On_Off_changed()
{
  
  if(document.getElementById('On_Off_Input').checked === true)
  {
    document.getElementById('On_Off_Value').innerHTML = 'ON'
    document.getElementById("O2_Input").disabled = false;
    document.getElementById("Flow_in_Input").disabled = false;
    UploadUserInputData(on_off_update_url,1);
  }
  else
  {
    document.getElementById('On_Off_Value').innerHTML = 'OFF'
    document.getElementById("O2_Input").disabled = true;
    document.getElementById("Flow_in_Input").disabled = true;
    document.getElementById('O2_Input').value = 21;
    document.getElementById('Flow_in_Input').value = 0;
    UploadUserInputData(on_off_update_url,0);
    O2changed();
    Flow_in_changed();
    
  }
  
}

// Check new Sensor data from database every 100 ms
setInterval("readAllSensorData();",100);

// Check new Sensor data from database every 100 ms
setInterval("readAllInputData();",500);
