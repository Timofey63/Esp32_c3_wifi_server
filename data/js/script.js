async function loadState() 
{
    const response = await fetch("/api");
    const data = await response.json();
    document.getElementById("status").innerText =
        data.ledState ? "ВКЛЮЧЕН" : "ВЫКЛЮЧЕН";
}

async function setLed(state) 
{
    await fetch(state ? "/on" : "/off");
    loadState();
}

setInterval(loadState, 1000);
loadState();