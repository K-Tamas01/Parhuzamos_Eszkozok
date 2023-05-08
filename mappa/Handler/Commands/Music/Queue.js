const error = require('../../../Response/Error');
const success = require('../../../Response/Success');

module.exports = {
	name:'queue',
	category:'Service',
	description:'$queue (Lejátszási lista megjelenítése)',
	isload: true,
	run: async (bot, msg, args, guildqueue, user) => {
		if (args.length === 0) {
			if (guildqueue !== undefined && guildqueue !== null) {
				if (user === undefined) {
					success.run(msg, 1, guildqueue, args, user);
				}
				else {
					success.run(msg, 1, guildqueue, args, user);
				}
			}
			else {
				error.run(msg, 4, guildqueue, args);
			}
		}
		else {
			error.run(msg, 1, guildqueue, args);
		}
	},
};