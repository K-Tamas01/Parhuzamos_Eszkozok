const { readdirSync } = require('fs');
const ascii = require('ascii-table');

const table = new ascii('Parancsok');
table.setHeading('Parancs', 'Státusz');

module.exports = (client) => {
	readdirSync('./Handler/Commands/').forEach(dir => {
		const commands = readdirSync(`./Handler/Commands/${dir}`).filter(file => file.endsWith('.js'));

		for (const files of commands) {
			const pull = require(`../Handler/Commands/${dir}/${files}`);

			if (pull.isload) {
				client.command.set(pull.name, pull);
				table.addRow(pull.name, 'Loaded');
			}
			else {
				table.addRow(pull.name, 'Unloaded');
			}
			if (pull.aliases && Array.isArray(pull.aliases)) pull.aliases.forEach(() => client.aliases.set(pull.name));
		}
	});
	console.log(table.toString());
};