chrome.runtime.onMessage.addListener((message, sender, reply) => {
  if (message.kind === "tabs") {
    chrome.tabs.query({}).then(tabs => reply({ tabVisible: tabs.some(t => t.id === sender.tab.id) }), error => reply({ error: String(error) }));
    return true;
  }
  if (message.kind === "groups") {
    chrome.tabGroups.query({}).then(groups => reply({ groupCount: groups.length }), error => reply({ error: String(error) }));
    return true;
  }
});
