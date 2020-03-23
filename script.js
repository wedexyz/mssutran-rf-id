
var firebaseConfig = {
    databaseURL: "https://user-49128.firebaseio.com",
  
	};
	firebase.initializeApp(firebaseConfig);
	function tampilData()
	{

		var dbRef = firebase.database();
		var statusAlat = dbRef.ref("id");
		var table = document.getElementById("tabel-siswa").getElementsByTagName('tbody')[0];
		$("#tabel-siswa").find("tr:gt(0)").remove();
		statusAlat.on("child_added", function(data, prevChildKey) {
		   	var newstatusAlat = data.val();
		   	var row = table.insertRow(table.rows.length);
		   	var cell1 = row.insertCell(0);
		   	cell1.innerHTML = newstatusAlat; 
		   	
		});

	}

