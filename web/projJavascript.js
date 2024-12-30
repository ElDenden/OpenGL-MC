var allPages = ["homePage", "objectives", "demo", "considerations"];
var currentPage = 0;

function displayAnimalList(animalListChoice){
	let animalList = document.getElementById(animalListChoice.value);
	let animalLists = document.getElementsByName(animalListChoice.name);
	if(animalListChoice.checked){
		
		for(let index = 0; index < animalLists.length; index++){
			if(animalLists[index].value !== animalListChoice.value && animalLists[index].value !== "DNM"){
				let animalList = document.getElementById(animalLists[index].value);
				animalList.style.display = "none";
			}
		}
		
		if(animalListChoice.value !== "DNM")
			animalList.style.display = "inline";
	}
	else{
		animalList.style.display = "none";
	}
}

function displaySecondList(checkedElement){
	let rAnimalsArr = document.getElementsByName("rAnimals");
	if(checkedElement.checked){
		for(let index = 0; index < rAnimalsArr.length; index++){
			document.getElementById(rAnimalsArr[index].value + "2").style.display = "inline";
		}
	}
	else{
		for(let index = 0; index < rAnimalsArr.length; index++){
			document.getElementById(rAnimalsArr[index].value + "2").style.display = "none";
		}
	}
}

function changePage(nextPage){
	if(nextPage.value === currentPage)
		return;
	
	document.getElementById(allPages[currentPage]).style.display = "none";
	
	currentPage = nextPage.value;
	
	document.getElementById(allPages[currentPage]).style.display = "block";
}