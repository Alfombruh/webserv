window.addEventListener("load", function () {
  console.log("hola");
  const input = document.querySelector("#sexo");
  input.addEventListener("change", () => {
    const files = input.files;
    if (files) {
		let _datos = {
			titulo: "foo",
			principal: "bar", 
			Id:1
		  }
      let data = new FormData();
      data.append("file", files[0], files[0].name);
      fetch("/upload", {
        method: "POST",
		// body: JSON.stringify(_datos),
        body: "1234",
        headers: { "Content-type": "image/png" },
      })
        .then((response) => console.log(response))
        .catch((err) => console.log(err));
      //   axios
      //     .post("/upload", data, {
      //       headers: {
      //         accept: "application/json",
      //         "Content-Type": `image/png`,
      //       },
      //     })
      //     .then(function (response) {
      //       console.log(response);
      //     })
      //     .catch(function (error) {
      //       console.log(error);
      //     });
      console.log(files[0]);
    }
  });
});
// https://breakingbadapi.com/documentation
/*---------------------------------------top 9---------------------------------------------*/
{
  axios
    .get("https://www.breakingbadapi.com/api/characters?limit=9&offset=0")
    .then(function (response) {
      for (let i = 0; i < 9; i++) {
        let characterName = response.data[i].name;
        let characterImg = response.data[i].img;
        let characterNikname = response.data[i].nickname;
        let characterOcupation = response.data[i].occupation;

        newDiv = document.createElement("div");
        newNameHeader = document.createElement("H4");
        newCharacterImage = document.createElement("img");
        newNiknameP = document.createElement("p");
        newOcupationP = document.createElement("p");

        newDiv.className = "inportedDiv";
        newNameHeader.innerHTML = characterName;
        newCharacterImage.src = characterImg;
        newNiknameP.innerHTML = "Nikname: " + characterNikname;
        newOcupationP.innerHTML = "Ocupation: " + characterOcupation;

        newDiv.appendChild(newNameHeader);
        newDiv.appendChild(newCharacterImage);
        newDiv.appendChild(newNiknameP);
        newDiv.appendChild(newOcupationP);

        document.getElementById("topNineCharacters").appendChild(newDiv);
      }
    })
    .catch(function (error) {
      // handle error
    });
}
/*---------------------------------------random walter/jesse quotes---------------------------------------------*/
{
  axios
    .get("https://www.breakingbadapi.com/api/quote/random?author=Walter+White")
    .then(function (response) {
      let walterRamndomQuote = response.data[0].quote;
      document.getElementById("walterQuote").innerHTML =
        '"' + walterRamndomQuote + '"';
    })
    .catch(function (error) {
      // handle error
      console.log(error);
    });

  axios
    .get("https://www.breakingbadapi.com/api/quote/random?author=Jesse+Pinkman")
    .then(function (response) {
      let jesseRamndomQuote = response.data[0].quote;
      document.getElementById("jesseQuote").innerHTML =
        '"' + jesseRamndomQuote + '"';
    })
    .catch(function (error) {
      // handle error
      console.log(error);
    });
}
printFavList();

/*---------------------------------------user search---------------------------------------------*/

function myFunction() {
  document.getElementById("sherchedCharacter").innerHTML = "";

  let userCharacter = document.getElementById("userCharacter").value;

  let petition =
    "https://www.breakingbadapi.com/api/characters?name=" + userCharacter;

  axios
    .get(petition)
    .then(function (response) {
      let characterName = response.data[0].name;
      let characterImg = response.data[0].img;
      let characterNikname = response.data[0].nickname;
      let characterOcupation = response.data[0].occupation;
      let characterId = response.data[0].char_id;

      newDiv = document.createElement("div");
      newNameHeader = document.createElement("H4");
      newCharacterImage = document.createElement("img");
      newNiknameP = document.createElement("p");
      newOcupationP = document.createElement("p");
      newFavButton = document.createElement("button");

      newDiv.className = "inportedDiv";
      newNameHeader.innerHTML = characterName;
      newCharacterImage.src = characterImg;
      newNiknameP.innerHTML = "Nikname: " + characterNikname;
      newOcupationP.innerHTML = "Ocupation: " + characterOcupation;
      newFavButton.innerHTML = "Fav";
      newFavButton.id = characterId;
      // newFavButton.addEventListener("click", function () { addToFav(characterName, characterImg, characterNikname, characterOcupation, characterId)});
      newFavButton.addEventListener("click", function () {
        favorites(response.data[0]);
      });

      newDiv.appendChild(newNameHeader);
      newDiv.appendChild(newCharacterImage);
      newDiv.appendChild(newNiknameP);
      newDiv.appendChild(newOcupationP);
      newDiv.appendChild(newFavButton);

      document.getElementById("sherchedCharacter").appendChild(newDiv);
    })
    .catch(function (error) {
      // handle error
      newError = document.createElement("H4");
      newError.innerHTML = "Sorry, That character does not exist";
      document.getElementById("sherchedCharacter").appendChild(newError);
    });
}

/*---------------------------------------add to favs---------------------------------------------*/

// function FavObjectList(characterName, characterImg, characterNikname, characterOcupation, characterId){
//   let favCharacters=[];
//   let favCharacter={
//     characterName:characterName,
//     characterImg:characterImg,
//     characterNikname:characterNikname,
//     characterOcupation:characterOcupation,
//     characterId:characterId
//   }
//   favCharacters.push(favCharacter);

//   newFavButton.className=characterIdGreen;

//   localStorage.setItem(favCharacters);
//   console.log(localStorage.getItem(favCharacters));

// }

/*---------------------------------------pruebas---------------------------------------------*/

function favorites(object) {
  let dataList;
  if (!checkIfExist(object)) {
    if (localStorage.getItem("data") === null) {
      dataList = [];
    } else {
      let ls = localStorage.getItem("data");
      dataList = JSON.parse(ls);
    }
    dataList.push(object);
    dataList = JSON.stringify(dataList);
    localStorage.setItem("data", dataList); // para guardar. Set es un insert en el local storage
    // let favorite = localStorage.getItem('data');    // para sacar
    // console.log(favorite);
    // debugger;
    //Funcion para pintar lista favoritos
    printFavList();
  }
}
function printFavList() {
  if (localStorage.getItem("data") !== null) {
    let ls = localStorage.getItem("data");
    let dataList = JSON.parse(ls);
    document.getElementById("favoriteCharacters").innerHTML = "";
    console.log("entra en print");

    dataList.forEach((element) => {
      let characterName = element.name;
      let characterImg = element.img;
      let characterNikname = element.nickname;
      let characterOcupation = element.occupation;
      let characterIdGreen = element.char_id;

      newInFavDiv = document.createElement("div");
      newInFavNameHeader = document.createElement("H4");
      newInFavCharacterImage = document.createElement("img");
      newInFavNiknameP = document.createElement("p");
      newInFavOcupationP = document.createElement("p");
      newInFavButton = document.createElement("button");

      newInFavDiv.className = "inportedDiv";
      newInFavNameHeader.innerHTML = characterName;
      newInFavCharacterImage.src = characterImg;
      newInFavNiknameP.innerHTML = "Nikname: " + characterNikname;
      newInFavOcupationP.innerHTML = "Ocupation: " + characterOcupation;
      newInFavButton.innerHTML = "Fav";
      newInFavButton.className = "characterButtonGreen";

      newInFavButton.setAttribute("data", JSON.stringify(element));
      // newFavButton.addEventListener("click", function () { addToFav(characterName, characterImg, characterNikname, characterOcupation, characterId)});

      newInFavDiv.appendChild(newInFavNameHeader);
      newInFavDiv.appendChild(newInFavCharacterImage);
      newInFavDiv.appendChild(newInFavNiknameP);
      newInFavDiv.appendChild(newInFavOcupationP);
      newInFavDiv.appendChild(newInFavButton);

      document.getElementById("favoriteCharacters").appendChild(newInFavDiv);
    });

    let buttons = document.getElementsByClassName("characterButtonGreen");

    for (let i = 0; i < buttons.length; i++) {
      buttons[i].addEventListener("click", function () {
        let data = JSON.parse(this.getAttribute("data"));
        removeFromFavorites(data);
        printFavList();
      });
    }
  }
}

function checkIfExist(selectedFav) {
  if (localStorage.getItem("data") !== null) {
    let favList = JSON.parse(localStorage.getItem("data"));
    let isFavved = false;
    favList.forEach((element) => {
      if (element.name === selectedFav.name) {
        isFavved = true;
      }
    });
    if (isFavved) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

function removeFromFavorites(selectedFav) {
  let lsFavList = JSON.parse(localStorage.getItem("data"));
  let cont = 0;
  let positionToDelete;
  lsFavList.forEach((element) => {
    if (element.name === selectedFav.name) {
      positionToDelete = cont;
    }
    cont++;
  });
  lsFavList.splice(positionToDelete, 1);
  let newlsFavList = JSON.stringify(lsFavList);
  localStorage.setItem("data", newlsFavList);
}
