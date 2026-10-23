# Bounded termination observation audit

A peer webextras collection had a real FATAL after Browser.close, despite later process absence. Therefore process absence cannot alone establish clean termination.

The eleven preserved logs from eight named sessions result folders in fatal-log-scan.json contain no matches for the explicitly listed fatal/signal signatures. inputs.json pins those exact logs. This scan is not proof that no crash occurred: OS exit status and a positive NSApplicationWillTerminateNotification were not independently observed in these sessions fixtures.

Retain only the direct gates: shutdown request/nonce/PID acknowledgment, recorded process/helper disappearance, no fallback TERM where recorded, and profile/cache restoration. “Natural” in older reports means this bounded no-forced-termination sequence, not an observed normal OS exit status. Restore histories, callback counts and supplemental restart/new-PID results remain directly observed; this wording correction does not manufacture a new runtime result. No app, source, shared file or original fixture was changed or rerun.
