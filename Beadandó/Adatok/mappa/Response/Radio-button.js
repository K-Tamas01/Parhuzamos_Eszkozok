const { EmbedBuilder, ActionRowBuilder, ButtonBuilder, ButtonStyle } = require('discord.js');

let message_emb = undefined;

module.exports = {
	run:async (msg, song, newsong) => {
		if (song === undefined && newsong === undefined) {
			message_emb.delete();
		}
		else if (newsong !== undefined) {

			const content = new EmbedBuilder()
				.setTitle(`Most játszom éppen: ${newsong.name}`)
				.setColor('#0099ff')
				.setColor('#FF1493');

			message_emb.edit({ embeds: [content] });

		}
		else {
			const buttons = new ActionRowBuilder()
				.addComponents(
					new ButtonBuilder()
						.setCustomId('Play')
						.setLabel('Play')
						.setStyle(ButtonStyle.Success),

					new ButtonBuilder()
						.setCustomId('Leave')
						.setLabel('Leave')
						.setStyle(ButtonStyle.Danger),

					new ButtonBuilder()
						.setCustomId('Skip')
						.setLabel('Skip')
						.setStyle(ButtonStyle.Secondary),

					new ButtonBuilder()
						.setCustomId('Pause')
						.setLabel('Pause')
						.setStyle(ButtonStyle.Success),
				);
			const buttons2 = new ActionRowBuilder()
				.addComponents(
					new ButtonBuilder()
						.setCustomId('volume+')
						.setLabel('Hangerő+')
						.setStyle(ButtonStyle.Success),

					new ButtonBuilder()
						.setCustomId('volume-')
						.setLabel('Hangerő-')
						.setStyle(ButtonStyle.Danger),

					new ButtonBuilder()
						.setCustomId('Queue')
						.setLabel('Queue')
						.setStyle(ButtonStyle.Secondary),
				);

			const content = new EmbedBuilder()
				.setTitle(`Most játszom éppen: ${song.name}`)
				.setColor('#0099ff')
				.setColor('#FF1493');

			message_emb = await msg.channel.send({ ephemeral: true, embeds: [content], components: [buttons, buttons2] });
		}
	},
};