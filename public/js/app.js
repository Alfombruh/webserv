window.addEventListener("load", function () {
  //POST GALERY
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
  input.addEventListener("change", () => {
    if (!window.FileReader) return; // Browser is not compatible

    const feedback = document.querySelector("#post-input-feedback");
    const files = input.files;
    if (!files | !files[0]) return;

    let reader = new FileReader();

    let f = input.files[0];

    reader.onloadend = function () {
      var binaryImg = convertDataURIToBinary(reader.result);
      let fetchPostGalery = {
        method: "POST",
        headers: {
          "Content-Type": "image/png",
          Connection: "close",
          Accept: "*/*",
        },
        body: binaryImg,
      };
      fetch("/galery?filename=" + files[0].name, fetchPostGalery)
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
});

//DELETE GALERY
function deleteGalery() {
  const input = document.querySelector("#delete-input").value;
  const feedback = document.querySelector("#delete-input-feedback");
  if (!input.endsWith(".jpg") && !input.endsWith(".png")) {
    feedback.innerHTML =
      "el nomre del archivo no es valido: example.png | example.jpg";
	  return ;
  }
  let fetchDeleteGalery = {
    method: "DELETE",
    headers: {
      "Content-Type": "text/plain",
      Connection: "close",
      Accept: "text/plain",
    },
  };
  console.log(input);

  const url = "/galery?filename=" + input;
  fetch(url, fetchDeleteGalery)
    .then((res) => {
      feedback.innerHTML =
        res.status === 200
          ? "Imagen " + input + " borrada"
          : "Imagen " + input + " no existe";
    })
    .catch((error) => error);
}

