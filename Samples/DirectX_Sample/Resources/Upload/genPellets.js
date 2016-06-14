
function placePellets()
{
    var player = document.getElementById("player");
	numPellets = document.getElementById('numPellets').value
	posData = document.getElementById('pelletLocs').innerText;
	planet = document.getElementById("pelletDiv");
	pelletTemplate = document.createElement("img");
	pelletTemplate.setAttribute("class","pellet");
	pelletTemplate.setAttribute("src", "pellet.png");
	if (window.innerWidth < 500) {
	    pelletTemplate.style.width = "16px";
	    pelletTemplate.style.height = "16px";
	    pelletTemplate.style.margin = "-8px -8px";
	    player.style.width = "16px";
	    player.style.height = "16px";
	    player.style.margin = "-8px -8px";
	}
	
	//Add or remove pellets until we have enough:
	var stop=new Number(0);
	while(planet.getElementsByClassName("pellet").length < numPellets && stop < numPellets)
	{
		planet.appendChild(pelletTemplate.cloneNode());
		stop= stop+1;
	}
	while(planet.getElementsByClassName("pellet").length > numPellets)
	{
		planet.removeChild(planet.getElementsByClassName("pellet")[0]);
	}
	pellets = planet.getElementsByClassName("pellet");
	pelletNums = posData.split(',');
	var maxr = 0;
	
	var worldHalf = Math.min(window.innerHeight, window.innerWidth) / 4;
	var worldCenter = document.getElementById('world').getBoundingClientRect();
	for(var i=0; i <= numPellets; i++)
	{
		if(pelletNums[i*2] > maxr)
			maxr = pelletNums[i*2];
		r = parseFloat(pelletNums[i * 2]) * (worldHalf / 3);
		t = parseFloat(pelletNums[i * 2 + 1]) * 6.28;
		if(i < numPellets)
		{
		    pellets[i].style.top = (worldCenter.top +   window.innerHeight/4 + Math.sin(t) * r) + "px";
		    pellets[i].style.left = (worldCenter.left + window.innerWidth/4 + Math.cos(t) * r) + "px";
		}
		else
		{
			player.style.top = (worldCenter.top + window.innerHeight/4 + Math.sin(t) * r) + "px";
			player.style.left = (worldCenter.left + window.innerWidth/4 + Math.cos(t) * r) + "px";
		}
	}
}
