document.documentElement.dataset.arcCoreScenario = "extension-7f3a";
chrome.runtime.sendMessage({ kind: "tabs" }, response => {
  document.documentElement.dataset.arcCoreTabsProbe = JSON.stringify(response);
});
chrome.runtime.sendMessage({ kind: "groups" }, response => {
  document.documentElement.dataset.arcCoreGroupsProbe = JSON.stringify(response);
});
