const cursoCDIOgrupo = 'cdiocurso2023g09';

//dweet.io/dweet/for/cdiocurso2023g09
//dweet.io:443/get/dweets/for/

getData();

setInterval(getData(), 3000);

async function getData() {
  const resp = await fetch('https://dweet.io/dweet/for/' + cursoCDIOgrupo);
  console.log(resp);
  console.log(resp.body); // locked?
}
