// Which web platform features the engine exposes to pages. Many need Chrome UI (choosers, pickers, dialogs)
// that Arc's windows don't have, so a feature Arc's engine disabled must stay disabled on ours, and one it
// enabled needs a working (or safely failing) UI path. The detected set is one step, compared with the golden.
import type { ScenarioContext } from "../launch-slice/run";
import { run, page } from "./native";

const PROBES: Record<string, string> = {
  fedcm: "typeof IdentityCredential !== 'undefined'",
  fedcmDisconnect: "typeof IdentityCredential !== 'undefined' && typeof IdentityCredential.disconnect === 'function'",
  digitalCredentials: "typeof DigitalCredential !== 'undefined'",
  webauthn: "typeof PublicKeyCredential !== 'undefined'",
  paymentRequest: "typeof PaymentRequest !== 'undefined'",
  webShare: "typeof navigator.share === 'function'",
  notifications: "typeof Notification !== 'undefined'",
  bluetooth: "'bluetooth' in navigator",
  usb: "'usb' in navigator",
  hid: "'hid' in navigator",
  serial: "'serial' in navigator",
  fileSystemAccess: "typeof showOpenFilePicker === 'function'",
  eyeDropper: "typeof EyeDropper !== 'undefined'",
  localFonts: "typeof queryLocalFonts === 'function'",
  documentPiP: "'documentPictureInPicture' in window",
  webgpu: "'gpu' in navigator",
  webxr: "'xr' in navigator",
  speechRecognition: "typeof webkitSpeechRecognition !== 'undefined'",
  speechSynthesis: "'speechSynthesis' in window",
  wakeLock: "'wakeLock' in navigator",
  contacts: "'contacts' in navigator",
  idleDetection: "typeof IdleDetector !== 'undefined'",
  windowManagement: "typeof getScreenDetails === 'function'",
  storageAccess: "typeof document.requestStorageAccess === 'function'",
  topics: "typeof document.browsingTopics === 'function'",
  protectedAudience: "typeof navigator.joinAdInterestGroup === 'function'",
  sharedStorage: "'sharedStorage' in window",
  privateAggregation: "'privateAggregation' in window",
  compute: "'computePressure' in window || typeof PressureObserver !== 'undefined'",
  translatorApi: "typeof Translator !== 'undefined'",
  summarizerApi: "typeof Summarizer !== 'undefined'",
  languageModel: "typeof LanguageModel !== 'undefined'",
  launchQueue: "'launchQueue' in window",
  badging: "typeof navigator.setAppBadge === 'function'",
  clipboardItem: "typeof ClipboardItem !== 'undefined'",
  credentialsGet: "typeof navigator.credentials?.get === 'function'",
};

export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await page(c);
    const expression = `JSON.stringify({${Object.entries(PROBES).map(([k, v]) => `${k}: (()=>{try{return ${v}}catch(e){return 'error'}})()`).join(",")}})`;
    const result = JSON.parse(String(c.evaluate(t, expression))) as Record<string, unknown>;
    const enabled = Object.keys(result).filter(k => result[k] === true).sort();
    const disabled = Object.keys(result).filter(k => result[k] !== true).sort();
    c.record("enabled features", "pass", enabled.join(","));
    c.record("disabled features", "pass", disabled.join(","));
  });
}
