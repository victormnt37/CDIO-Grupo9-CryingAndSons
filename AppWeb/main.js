const url = 'https://dweet.io:443/get/dweets/for/cdiocurso2023g09';

let valorHumedad = 'Inactivo';
let valorTemperatura = 'Inactivo';
let valorSalinidad = 'Inactivo';
let valorPH = 'Inactivo';
let valorLuz = 'Inactivo';

//dweet.io/dweet/for/cdiocurso2023g09
//dweet.io:443/get/dweets/for/

// getData();

// setInterval(getData(), 3000);

// async function getData() {
//   const resp = await fetch(url);
//   console.log(resp);
//   console.log(resp.body); // locked?
// }

fetch(url)
  .then(function (response) {
    return response.json();
  })
  .then(function (data) {
    console.log(data.with[0].content); // el 0 ha de cambiar al último posible? o siempre es el 0, una de dos
    console.log(data.with[0].content);
    valorHumedad = data.with[0].content.field1; // Temperatura
    valorSalinidad = data.with[0].content.field2; // Salinidad
    valorHumedad = data.with[0].content.field3; // Humedad
    valorHumedad = data.with[0].content.field4; // PH
    valorLuz = data.with[0].content.field5; // Luz
  })
  .catch(function (error) {
    console.log('Hubo un problema con la solicitud fetch: ', error);
  });
