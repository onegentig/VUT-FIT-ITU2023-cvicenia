/* eslint no-alert: "off" */
/* eslint no-unused-vars: "off" */

/* === Typy === */

/**
 * @typedef {Object} ChatMessage
 * @property {Number} id ID správy
 * @property {String} login Uživateľské meno
 * @property {String} dts Čas odoslania
 * @property {String} cnt Obsah
 */

/* === Konštanty === */

/* Konfigurácia */

const API_URL = 'https://pckiss.fit.vutbr.cz/itu/api.php';
const POOLING_TIME = 1000; // v milisekundách
const USERNAME = 'xnotme69';

/* Systémové konštanty */

const READY_STATE_DONE = 4;
const STATUS_OK = 200;

/* === Hlavné funkcie === */

/**
 * Zkonštruuje objekt XMLHttpRequest (pre kompatibilitu s rôznymi prehliadačmi).
 * @see https://moodle.vut.cz/mod/page/view.php?id=320744#yui_3_17_2_1_1698178339099_103
 * @returns {XMLHttpRequest} AJAX HttpRequest objekt
 */
function createXmlHttpRequestObject () {
     /** @type {XMLHttpRequest | ActiveXObject | undefined} */
     let req;

     if (window.XMLHttpRequest)
          /* Moderné prehliadače */
          req = new XMLHttpRequest();
     else if (window.ActiveXObject)
          /* IE6 alebo staršie */
          try {
               // eslint-disable-next-line no-undef
               req = new ActiveXObject('Microsoft.XMLHTTP');
          } catch {
               // Ignorovať chybu
          }

     if (!req)
          displayError('Failed to create an XMLHttpRequest object.');

     // @ts-ignore pretože sa mi to nechce riešiť
     return req;
}

/**
 * Pošle novú správu na chat server.
 * @returns {false} false (pre zamedzenie východzieho správania formulárov)
 */
function uploadData () {
     updateStatus('uploadData()');

     const req = createXmlHttpRequestObject();
     const input = /** @type {HTMLInputElement} */ (document.querySelector('#newMessageString'));
     const data = input.value;

     /* Vyčistiť input */
     input.value = '';

     try {
          /* Vytvoriť a odoslať požiadavok */
          req.open('POST', API_URL, true); // Async. POST na API_URL
          req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
          req.send(`user=${USERNAME}&data=${encodeURIComponent(data)}`);
     } catch (error) {
          displayError(error.toString());
          return false;
     }

     /* Spracovať odpoveď, keď príde */
     req.addEventListener('readystatechange', () => {
          if (req.readyState === READY_STATE_DONE) {
               if (req.status === STATUS_OK) return; // Všetko OK

               // Nie je OK :(
               const res = JSON.parse(req.responseText);
               displayError(res.error);
          }
     });

     return false;
}

/**
 * Stiahne zo servera posledné správy.
 * @returns {void}
 */
function downloadData () {
     updateStatus('downloadData()');

     const req = createXmlHttpRequestObject();
     req.open('GET', API_URL, true); // Async. GET na API_URL

     req.addEventListener('readystatechange', () => {
          if (req.readyState === READY_STATE_DONE) {
               if (req.status !== STATUS_OK)
                    return displayError(req.statusText);

               const res = JSON.parse(req.responseText);
               displayMessages(res); // Zobraziť správy
          }
     });

     req.send();
}

/**
 *
 * @param {Array<ChatMessage>} msgs Pole správ
 * @returns {void}
 */
function displayMessages (msgs) {
     const chat = document.querySelector('#chatArea');

     for (const msg of msgs) {
          /**
           * K div-om správ pridávam id pre jednoduché odfiltrovanie
           * už spracovaných správ, a nemusel som pridávať globálne
           * `lastId` alebo `lastTime` alebo whatever.
           */
          if (document.querySelector(`#message-${msg.id}`)) continue;

          /* Vytvoriť div pre správu */
          const msgDiv = document.createElement('div');
          msgDiv.id = `message-${msg.id}`;

          /* Obsah */
          msgDiv.innerHTML = `
            <div class="message-header">
                <span class="message-username">${msg.login}</span>
                <span class="message-dts">${msg.dts}</span>
            </div>
            <div class="message-content">${msg.cnt}</div>
        `;

          /* Pridať do chatu */
          chat.append(msgDiv);
     }

     /* Scrollnúť na spodok */
     chat.scrollTop = chat.scrollHeight;
}

/* === Pomocné funkcie === */

/**
 * Aktualizuje `status` element, zobrazujúci naposledy vykonanú akciu.
 * @param {String} message Správa na zobrazenie
 * @returns {void}
 */
function updateStatus (message) {
     document.querySelector('#status').innerHTML = message;
}

/**
 * Zobrazí chybové hlásenie (tu len alert, v reálnom projekte by sa použil
 * nejaký popup alebo proste niečo krajšie :>) a zaloguje chybu.
 * @param {String} message Chybové hlásenie
 * @returns {void}
 */
function displayError (message) {
     console.error(message);
     alert(message);
}

/* === Koreňové volania === */

// Pooling nových správ
setInterval(downloadData, POOLING_TIME);
