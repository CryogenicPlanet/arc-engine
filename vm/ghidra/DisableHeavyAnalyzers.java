// Pre-analysis script: turns off analyzers that spawn decompiler processes across the whole
// program (the memory spike that got the first run OOM-killed). Used only for the retry.
// @category Arc

import ghidra.app.script.GhidraScript;

public class DisableHeavyAnalyzers extends GhidraScript {
	@Override
	protected void run() throws Exception {
		for (String name : new String[] { "Decompiler Switch Analysis", "Decompiler Parameter ID" }) {
			setAnalysisOption(currentProgram, name, "false");
			println("DisableHeavyAnalyzers: " + name + " = false");
		}
	}
}
