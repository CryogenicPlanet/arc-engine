# password-save golden (accepted by the lead, 2026-10-23)

Recorded twice on Arc's 1.170 engine (trace156); both runs passed every page step and delivered
`webContents:didSubmitCredentials:` with an `ArcLoginCredentialsSaveRequest` (app reads `isUpdate` = NO).
The final quit step fails on Arc's own engine in both runs: shutdown starts (`shutdown-type silent_exit`)
but the process takes longer than 15 s to exit after a pending password offer, even with the tab closed.
That is the reference engine's behavior, not a boundary call, so the transcript is accepted with that step
failing. Judge our engine on the transcript diff; a faster quit on ours is not a regression.
