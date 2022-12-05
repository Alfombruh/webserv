
window.addEventListener("load", function () {
  const input = document.querySelector("#sexo");
  function convertDataURIToBinary(dataURI) {
    var BASE64_MARKER = ";base64,";
    var base64Index = dataURI.indexOf(BASE64_MARKER) + BASE64_MARKER.length;
    var base64 = dataURI.substring(base64Index);
    var raw = window.atob(base64);
    var rawLength = raw.length;
    var array = new Uint8Array(new ArrayBuffer(rawLength));

    for (i = 0; i < rawLength; i++) {
      array[i] = raw.charCodeAt(i);
    }
    return array;
  }
  input.addEventListener("change", () => {
    if (!window.FileReader) return; // Browser is not compatible

    const files = input.files;
    if (!files | !files[0]) return;

    let reader = new FileReader();

    let f = input.files[0];

    reader.onloadend = function () {
      var binaryImg = convertDataURIToBinary(reader.result);
      let fetchHasAccount = {
        method: "POST",
        headers: {
          "Content-Type": "image/png",
          Connection: "close",
          Accept: "*/*",
        },
        body: binaryImg,
      };
      fetch("/galery?filename=" + files[0].name, fetchHasAccount)
        .then((res) => res.json())
        .then((data) => {
          console.log(data);
        })
        .catch((error) => error);
    };
    reader.readAsDataURL(f);
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
