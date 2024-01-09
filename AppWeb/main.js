const url = 'https://dweet.io:443/get/dweets/for/cdiocurso2018g09';

let valorHumedad = 'Inactivo';
let valorTemperatura = 'Inactivo';
let valorSalinidad = 'Inactivo';
let valorPH = 'Inactivo';
let valorLuz = 'Inactivo';
let valorGPS = 'Inactivo';

const divHumedad = document.getElementById('humedad').lastElementChild;
const divTemperatura = document.getElementById('temperatura').lastElementChild;
const divSalinidad = document.getElementById('salinidad').lastElementChild;
const divPH = document.getElementById('PH').lastElementChild;
const divLuz = document.getElementById('luz').lastElementChild;
const divGPS = document.getElementById('GPS').lastElementChild;

actualizarDatos(); // Se actualizan los datos nada más se abre la página
setInterval(() => {
  // Y se repite la actualización cada 10 segundos
  actualizarDatos();
}, 10000);

function actualizarDatos() {
  fetch(url)
    .then(function (response) {
      return response.json();
    })
    .then(function (data) {
      valorTemperatura = data.with[0].content.field1; // Temperatura
      valorSalinidad = data.with[0].content.field2; // Salinidad
      valorHumedad = data.with[0].content.field3; // Humedad
      valorPH = data.with[0].content.field4; // PH
      valorLuz = data.with[0].content.field5; // Luz
      valorGPS = data.with[0].content.field6; // DPS

      valorHumedad == undefined // Si el dato no está definido pq no se envia por el api se muestra como inactivo
        ? (divHumedad.innerHTML = 'Inactivo')
        : (divHumedad.innerHTML = valorHumedad + ' %');

      valorTemperatura == undefined
        ? (divTemperatura.innerHTML = 'Inactivo')
        : (divTemperatura.innerHTML = valorTemperatura + ' ºC');

      valorSalinidad == undefined
        ? (divSalinidad.innerHTML = 'Inactivo')
        : (divSalinidad.innerHTML = valorSalinidad + ' g');

      valorPH == undefined
        ? (divPH.innerHTML = 'Inactivo')
        : (divPH.innerHTML = valorPH + ' pH');

      valorLuz == undefined
        ? (divLuz.innerHTML = 'Inactivo')
        : (divLuz.innerHTML = valorLuz);

      valorGPS == undefined
        ? (divGPS.innerHTML = 'Inactivo')
        : (divGPS.innerHTML = valorGPS);
    })
    .catch(function (error) {
      console.log('Hubo un problema con la solicitud fetch: ', error);
    });
}
