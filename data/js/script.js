async function loadState() {
    const response = await fetch("/api");
    const data = await response.json();
    document.getElementById("status").innerText =
        data.ledState ? "ВКЛЮЧЕН" : "ВЫКЛЮЧЕН";
}

async function setLed(state) {
    await fetch(state ? "/on" : "/off");
    loadState();
}

async function sendText() {
    const text = document.getElementById("textInput").value;
    await fetch("/sendText?value=" + encodeURIComponent(text));
    console.log("Sent:", text);
    document.getElementById("textInput").value = "";
}

setInterval(loadState, 1000);
loadState();