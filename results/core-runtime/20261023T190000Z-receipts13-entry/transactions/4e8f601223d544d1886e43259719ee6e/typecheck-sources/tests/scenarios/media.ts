import type { ScenarioContext } from "../launch-slice/run";
import { run, page } from "./native";
export async function playback(c: ScenarioContext) {
    const t = await page(c);
    const result = c.evaluate(t, `(async()=>{
      const canvas=document.createElement('canvas');canvas.width=64;canvas.height=64;document.body.append(canvas);
      const ctx=canvas.getContext('2d');let i=0;const timer=setInterval(()=>{ctx.fillStyle=i++%2?'red':'blue';ctx.fillRect(0,0,64,64)},30);
      const recorder=new MediaRecorder(canvas.captureStream(30),{mimeType:'video/webm;codecs=vp9'});const chunks=[];
      recorder.ondataavailable=e=>chunks.push(e.data);recorder.start();await new Promise(r=>setTimeout(r,700));
      await new Promise(r=>{recorder.onstop=r;recorder.stop()});clearInterval(timer);
      const video=document.createElement('video');video.id='scenario-video';video.muted=true;video.loop=true;video.controls=true;
      video.src=URL.createObjectURL(new Blob(chunks,{type:'video/webm'}));document.body.append(video);
      navigator.mediaSession.metadata=new MediaMetadata({title:'ArcCore media 7f3a',artist:'Local fixture',album:'Scenarios'});
      await video.play();await new Promise(r=>setTimeout(r,300));navigator.mediaSession.playbackState='playing';
      return {width:video.videoWidth,height:video.videoHeight,advancing:video.currentTime>0,paused:video.paused,title:navigator.mediaSession.metadata.title};
    })()`);
    c.assert("video and media metadata", JSON.stringify(result) === JSON.stringify({width:64,height:64,advancing:true,paused:false,title:"ArcCore media 7f3a"}), JSON.stringify(result));
    return t;
}
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await playback(c);
    c.evaluate(t, "document.getElementById('scenario-video').pause(); navigator.mediaSession.playbackState='paused'; 1");
    c.assert("pause video", c.evaluate(t, "document.getElementById('scenario-video').paused && navigator.mediaSession.playbackState==='paused'") === true);
  });
}
