const cursoCDIOgrupo = 'cdiocurso2018g09';

setInterval(getData(), 3000);

getData();

async function getData() {
  const resp = await fetch(
    'https://dweet.io:443/get/dweets/for/' + cursoCDIOgrupo
  );
  console.log(resp.body); // locked?
}
