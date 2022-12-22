window.addEventListener("load", function () {
  //POST gallery
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
  const input = document.querySelector("#image-post");
  if (input) {
    input.addEventListener("change", () => {
      if (!window.FileReader) return; // Browser is not compatible

      const feedback = document.querySelector("#post-input-feedback");
      const files = input.files;
      if (!files | !files[0]) return;

      let reader = new FileReader();

      let f = input.files[0];

      reader.onloadend = function () {
        var binaryImg = convertDataURIToBinary(reader.result);
        let fetchPostgallery = {
          method: "POST",
          headers: {
            "Content-Type": "image/png",
            Connection: "close",
            Accept: "*/*",
          },
          body: binaryImg,
        };
        fetch("/gallery-images/" + files[0].name, fetchPostgallery)
          .then((res) => {
            feedback.innerHTML =
              res.status === 201
                ? "Imagen " + files[0].name + " subida"
                : res.status === 409
                ? "Imagen " + files[0].name + " ya existe"
                : "Error en la llamada a api";
          })
          .catch((error) => error);
      };
      reader.readAsDataURL(f);
    });
  }

  function getCookie(cookieName) {
    let cookie = {};
    document.cookie.split(";").forEach(function (el) {
      let [key, value] = el.split("=");
      cookie[key.trim()] = value;
    });
    return cookie[cookieName];
  }
  function deleteCookie(name) {
    document.cookie =
      name + "=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;";
  }
  const cookie = getCookie("42webserv_session");
  const sessionDiv = document.querySelector("#user-session-div");
  if (sessionDiv && cookie == null) {
    sessionDiv.innerHTML =
      '<label for="name">Nombre</label><br>' +
      '<input type="text" id="login-name" name="name" maxlength="20" placeholder="John Doe"><br>' +
      '<label for="about">Sobre tí</label><br>' +
      '<input type="text" id="login-about" name="about" maxlength="50" placeholder="Me gustan las albondigas"><br>' +
      '<input onClick="logIn()"class="custom-input-file" type="submit" value="Iniciar sesion">' +
      '<p id="login-input-feedback" style="margin-top: 5px; font-size: 12px; color: red;"></p>';
  } else if (sessionDiv && cookie) {
    let fetchLogin = {
      method: "GET",
      headers: {
        Connection: "close",
        Accept: "*/*",
      },
    };
    fetch("/login", fetchLogin)
      .then((res) => (res.status === 302 ? res.json() : location.reload()))
      .then((data) => {
        if (data) {
          sessionDiv.innerHTML =
            "<h1>¡Hola " +
            decodeURI(data.name) +
            "!</h1><br>" +
            "<h3>" +
            decodeURI(data.about) +
            "</h3>" +
            '<input onClick="logOut()"class="custom-input-file" type="submit" value="Cerrar sesion">';
        } else deleteCookie("42webserv_session");
      })
      .catch((error) => console.log(error));
  }
});

function replaceAll(string, search, replace) {
  return string.split(search).join(replace);
}
function makeid(length) {
  var result = "";
  var characters =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  var charactersLength = characters.length;
  for (var i = 0; i < length; i++) {
    result += characters.charAt(Math.floor(Math.random() * charactersLength));
  }
  return result;
}

//LOG OUT
const logOut = () => {
  let fetchLogin = {
    method: "DELETE",
    headers: {
      Connection: "close",
      Accept: "*/*",
    },
  };
  fetch("/login", fetchLogin)
    .then((res) => {
      if (res.status === 200) location.reload();
      else console.log("bad request");
    })
    .catch((error) => console.log(error));
};

//LOG IN
const logIn = () => {
  const nameInput = document.querySelector("#login-name").value;
  const aboutInput = document.querySelector("#login-about").value;
  const feedback = document.querySelector("#login-input-feedback");
  if (nameInput === "" || aboutInput === "") {
    feedback.innerHTML = "rellena los campos";
    return;
  }
  feedback.innerHTML = "";
  let fetchLogin = {
    method: "POST",
    headers: {
      "Content-Type": "text/plain",
      Connection: "close",
      Accept: "*/*",
    },
    body: makeid(32),
  };
  //   replaceAll(nameInput, " ", "+");
  //   replaceAll(aboutInput, " ", "+");
  const route = "/login?name=" + nameInput + "&about=" + aboutInput;
  console.log(route);
  fetch("/login?name=" + nameInput + "&about=" + aboutInput, fetchLogin)
    .then((res) => {
      if (res.status === 201) location.reload();
      else console.log("bad request");
    })
    .catch((error) => error);
};

//DELETE gallery
function deletegallery() {
  const input = document.querySelector("#delete-input").value;
  const feedback = document.querySelector("#delete-input-feedback");
  if (!input.endsWith(".jpg") && !input.endsWith(".png")) {
    feedback.innerHTML =
      "el nomre del archivo no es valido: example.png | example.jpg";
    return;
  }
  let fetchDeletegallery = {
    method: "DELETE",
    headers: {
      "Content-Type": "text/plain",
      Connection: "close",
      Accept: "text/plain",
    },
  };

  const url = "/gallery-images/" + input;
  fetch(url, fetchDeletegallery)
    .then((res) => {
      feedback.innerHTML =
        res.status === 200
          ? "Imagen " + input + " borrada"
          : "Imagen " + input + " no existe";
    })
    .catch((error) => error);
}
