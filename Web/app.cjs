/* eslint no-alert: "off" */
/* eslint no-unused-vars: "off" */

/* === Globálne importy === */

/* eslint-disable no-var */

/** @global */
/** @type {import('dayjs') & import('dayjs/plugin/calendar')} */
var dayjs;

/* eslint-enable no-var */

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
const FLASH_COLOR = 'hsl(48, 100%, 67%)';
const POOLING_TIME = 400;
const STATUS_CHANGE_FLASH_TIME = 100;

/* Systémové konštanty */

const READY_STATE_DONE = 4;
const STATUS_OK = 200;

/** @type {Record<String, String>} */
let avatarsMap = {};

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
          req.send(`user=${getUsername()}&data=${encodeURIComponent(data)}`);
     } catch (error) {
          displayError(error.toString());
          return false;
     }

     /* Spracovať odpoveď, keď príde */
     req.addEventListener('readystatechange', () => {
          if (req.readyState === READY_STATE_DONE) {
               if (req.status === STATUS_OK) return; // Všetko OK

               // Nie je OK :(
               displayError('Could not upload data (check console). :(');
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
                    return displayError(req.statusText || 'Could not download data (is BE alive?)');

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

     // Scroll iba ak je view na spodku chatu.
     const shouldScroll = chat.scrollTop + chat.clientHeight > chat.scrollHeight - 100;

     for (const msg of msgs) {
          /**
           * K div-om správ pridávam id pre jednoduché odfiltrovanie
           * už spracovaných správ, a nemusel som pridávať globálne
           * `lastId` alebo `lastTime` alebo whatever.
           */
          if (document.querySelector(`#msg-${msg.id}`)) continue;

          /* Vytvoriť div pre správu */
          const msgDiv = document.createElement('div');
          msgDiv.id = `msg-${msg.id}`;

          /* Rozdielne správanie pre Bulma a plain */
          if (document.querySelector('body').classList.contains('use-bulma'))
               msgDiv.innerHTML = createMsgComponent(msg);
          else {
               /* day.js */
               let msgTimeStr = '';
               msgTimeStr = typeof dayjs === 'function' ? dayjs().calendar(dayjs(msg.dts)) : new Date(msg.dts).toLocaleString();

               /* Obsah */
               msgDiv.innerHTML = `
               <div class="msg-header">
                    <span class="msg-username">${msg.login}</span>
                    <span class="msg-dts">${msgTimeStr}</span>
               </div>
               <div class="msg-content">${decodeURIComponent(msg.cnt)}</div>
          `;
          }

          /* Pridať do chatu */
          chat.append(msgDiv);
     }

     /* Scrollnúť na spodok */
     if (shouldScroll) chat.scrollTop = chat.scrollHeight;
}

/* === Pomocné funkcie === */

/**
 * Aktualizuje `status` element, zobrazujúci naposledy vykonanú akciu.
 * @param {String} message Správa na zobrazenie
 * @returns {void}
 */
function updateStatus (message) {
     const stat = /** @type {HTMLElement} */ (document.querySelector('#status'));

     /* Zmeniť text */
     stat.innerHTML = message;

     /* Zmeniť farbu na žltú (na krátku chvíľu) */
     const pColor = /* stat.style.color */ 'hsl(0, 0%, 71%)';
     stat.style.color = FLASH_COLOR;
     setTimeout(() => {
          stat.style.color = pColor;
     }, STATUS_CHANGE_FLASH_TIME);
}

/**
 * Zobrazí chybové hlásenie (tu len alert, v reálnom projekte by sa použil
 * nejaký popup alebo proste niečo krajšie :>) a zaloguje chybu.
 * @param {String} message Chybové hlásenie
 * @returns {void}
 */
function displayError (message) {
     console.error(message);
     window.alert(message);
}

/**
 * Vytvorí HTML komponentu pre jednu chat správu.
 * @param {ChatMessage} msg
 * @returns {String} HTML komponenta ako string
 */
function createMsgComponent (msg) {
     /* Časový string (day.js, ak dostupné) */
     const timeStr = typeof dayjs === 'function' ? dayjs().calendar(dayjs(msg.dts)) : new Date(msg.dts).toLocaleString('cz-CZ', { dateStyle: 'short', timeStyle: 'short' });
     const avatar = avatarsMap[msg.login] || `https://ui-avatars.com/api/?name=${msg.login}&background=random&size=48&bold=true`;

     return `
     <div id="msg-${msg.id}" class="my-4 is-flex">
          <!-- Avatar -->
          <figure class="image is-48x48 mr-3">
               <img class="msg-img is-rounded" src="${avatar}" alt="${msg.login}" />
          </figure>

          <!-- Message box -->
          <div class="msg-box">
               <div class="is-flex mb-1">
                    <!-- Username -->
                    <strong class="msg-name has-text-secondary-dark">${msg.login || '▄█ ’M ▓▒'}</strong>
                    <!-- Timestamp -->
                    <span class="msg-time has-text-grey-dark is-size-7">${timeStr}</span>
               </div>

               <!-- Message Content -->
               <div class="msg-content">
                    ${msg.cnt}
               </div>
          </div>
     </div>
     `;
}

/**
 * Získa uživateľské meno z imputu.
 * @return {String} login/username
 */
function getUsername () {
     const input = /** @type {HTMLInputElement} */ (document.querySelector('#login'));
     const login = input.value;

     if (!login) {
          input.value = 'xlogin00';
          return 'xlogin00';
     }

     return login;
}

/* === Koreňové volania === */

// Pooling nových správ
setInterval(downloadData, POOLING_TIME);

// Načítanie profiloviek z `avatars.json`
fetch('avatars.json')
     .then(response => response.json())
     .then(data => avatarsMap = data)
     .catch(error => console.error(error));
